#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <epoxy/gl.h>

#include <pbase/rhc/log.h>
#include <pbase/rhc/alloc.h>
#include <pbase/mathc/float.h>

#include "visu/camera/perspective.h"
#include "visu/camera/orthogonal.h"
#include "visu/control/keyflow.h"
#include "visu/control/mouserotate.h"

#include "visu/visubase.h"

#define PRELOG "[Visu] "


static double monotonic() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (double) time.tv_sec + (double) time.tv_nsec / 1000000000.0;
}

typedef struct Visu {
    double time;
    GLuint vao;
    GLuint multisample_tex;
    GLuint multisample_color_buf;
    GLuint multisample_depth_buf;
    GLuint multisample_fbo;
    bool multisample_active;
    int last_width, last_height;

    VuCamera *camera;
    VuControl *control;

    // heap elements (need to be freed at visu kill...)
    VuVisuResource *resources;
    int resources_size;
    VuVisuResource *kill_stack;
    int kill_stack_size;

} Visu;

static int get_resource_pos(Visu *visu, VuVisuResource resource);

static void check_multisample(VuVisu *self, GLsizei width, GLsizei height);

static void init_multisample(VuVisu *self, GLsizei width, GLsizei height);

static void clear_multisample(VuVisu *self);

static void gl_msg_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar *message, const void *userParam);

VuVisu *vu_visu_new(bool orthogonal,
                    void (*render_function)(VuVisu *visu, void *user_data),
                    void *render_function_user_data) {
    VuVisu *self = p_rhc_calloc(sizeof *self);

    // private
    Visu *impl = p_rhc_calloc(sizeof *impl);
    self->impl = impl;

    impl->vao = -1;
    impl->multisample_tex = -1;
    impl->multisample_color_buf = -1;
    impl->multisample_depth_buf = -1;
    impl->multisample_fbo = -1;
    impl->multisample_active = false;
    impl->last_width = impl->last_height = -1;

    float z_near = VU_VISU_INIT_Z_NEAR;    // near clip distance [mm]
    float z_far = VU_VISU_INIT_Z_FAR; // far clip distance [mm]
    if (orthogonal) {
        impl->camera = vu_orthogonal_camera_new(z_near, z_far);
        impl->control = vu_mouse_rotate_control_new();
    } else {
        impl->camera = vu_perspective_camera_new(z_near, z_far);
        impl->control = vu_key_flow_control_new();
    }

    // public
    self->multisample = VU_VISU_INIT_MULTISAMPLE;

    self->camera = impl->camera;
    self->opt_control = impl->control;

    self->render_function = render_function;
    self->render_function_user_data = render_function_user_data;

    return self;
}

void vu_visu_kill(VuVisu **self_ptr) {
    if (!*self_ptr)
        return;
    VuVisu *self = *self_ptr;
    Visu *visu = self->impl;
    if (visu) {

        // VuCamera/Control_kill not necessary

        // kill render objects in the kill_stack (should be empty...)
        for (int i = 0; i < visu->kill_stack_size; i++) {
            VuVisuResource *res = &visu->kill_stack[i];
            res->kill(res);
        }

        // kill all remaining render objects
        for (int i = 0; i < visu->resources_size; i++) {
            VuVisuResource *res = &visu->resources[i];
            res->kill(res);
        }

        free(visu->resources);
        free(visu->kill_stack);

        p_rhc_free(self->impl);
    }
    p_rhc_free(self);
    *self_ptr = NULL;
}

void vu_visu_realize(VuVisu *self) {
    Visu *visu = self->impl;

    // check version
    log_info(PRELOG"OpenGL version: %s", glGetString(GL_VERSION));

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    if (major < 4 || (major == 4 && minor < 2)) {
        log_error(PRELOG"OpenGL version must be at least 4.2");
        exit(EXIT_FAILURE);
    }

    // error msg callback
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_msg_callback, NULL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_MULTISAMPLE);

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &visu->vao);
    glBindVertexArray(visu->vao);

    visu->time = monotonic();
}

void vu_visu_unrealize(VuVisu *self) {
    Visu *visu = self->impl;

    clear_multisample(self);

    // unload and kill render objects in the kill_stack
    for (int i = 0; i < visu->kill_stack_size; i++) {
        VuVisuResource *res = &visu->kill_stack[i];
        res->unload(res);
        res->kill(res);
    }
    visu->kill_stack_size = 0;

    // unload all remaining render objects
    for (int i = 0; i < visu->resources_size; i++) {
        VuVisuResource *res = &visu->resources[i];
        res->unload(res);
    }
}


void vu_visu_render(VuVisu *self, int width, int height) {
    Visu *visu = self->impl;

    double start_time = monotonic();
    double delta_time = start_time - visu->time;
    visu->time = start_time;

    GLint out_draw_buffer;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &out_draw_buffer);

    check_multisample(self, (GLsizei) width, (GLsizei) height);

    if (visu->multisample_active)
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, visu->multisample_fbo);

    // update camera
    self->camera->update_projection(self->camera, (float) width, (float) height);
    if (self->opt_control)
        self->opt_control->simulate(self->opt_control, self->camera, delta_time);

    // pre render settings
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(visu->vao);

    // unload and kill render objects in the kill_stack
    for (int i = 0; i < visu->kill_stack_size; i++) {
        VuVisuResource *res = &visu->kill_stack[i];
        res->unload(res);
        res->kill(res);
    }
    visu->kill_stack_size = 0;


    // set accessible render infos in VuVisu class
    self->screen_width = width;
    self->screen_height = height;
    self->delta_time = delta_time;

    // call user render function
    self->render_function(self, self->render_function_user_data);


    glFlush();

    // Filter multisampled buffer to output buffer
    if (self->multisample > 1) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, out_draw_buffer);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, visu->multisample_fbo);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glFlush();
    }
    visu->last_width = width;
    visu->last_height = height;
}

void vu_visu_3D(VuVisu *self) {
    glEnable(GL_DEPTH_TEST);
    self->view = mat4_inv(self->camera->pose);
    self->projection = self->camera->projection;
}

void vu_visu_2D(VuVisu *self, bool uniform_view) {
    glDisable(GL_DEPTH_TEST);
    self->view = mat4_eye();
    if (!uniform_view)
        self->projection = mat4_eye();
    else {
        log_wtf(PRELOG"uniform_view not implemented yet");
        self->projection = mat4_eye();
    }
}


void vu_visu_add_resource(VuVisu *self, VuVisuResource resource) {
    Visu *visu = self->impl;

    if (get_resource_pos(visu, resource) != -1) {
        log_warn(PRELOG"VisuResource already in the resources list");
        return;
    }
    visu->resources = p_rhc_realloc(visu->resources, sizeof *visu->resources * (visu->resources_size + 1));
    visu->resources[visu->resources_size++] = resource;
}

void vu_visu_kill_resource(VuVisu *self, VuVisuResource resource) {
    Visu *visu = self->impl;

    int pos = get_resource_pos(visu, resource);
    if (pos == -1) {
        log_error(PRELOG"VisuResource not available in the resources list");
        return;
    }

    // add to kill_stack
    visu->kill_stack = p_rhc_realloc(visu->kill_stack, sizeof *visu->kill_stack * (visu->kill_stack_size + 1));
    visu->kill_stack[visu->kill_stack_size++] = resource;

    // shrink the list
    while (pos < visu->resources_size - 1) {
        visu->resources[pos] = visu->resources[pos + 1];
        pos++;
    }
    visu->resources_size--;
}


static int get_resource_pos(Visu *visu, VuVisuResource res) {
    for (int i = 0; i < visu->resources_size; i++) {
        if (visu->resources[i].impl == res.impl)
            return i;
    }
    return -1;
}

static void check_multisample(VuVisu *self, GLsizei width, GLsizei height) {
    Visu *visu = self->impl;

    // clear multisample if it was active and set to 1 or the size changed
    if (visu->multisample_active) {
        if (self->multisample <= 1
            || visu->last_height != height
            || visu->last_width != width) {

            clear_multisample(self);
        }
    }

    // activate multisample if it was set above 1 and its not active yet
    if (self->multisample > 1 && !visu->multisample_active)
        init_multisample(self, (GLsizei) width, (GLsizei) height);
}

static void init_multisample(VuVisu *self, GLsizei width, GLsizei height) {
    Visu *visu = self->impl;

    GLint max_multisamples;
    glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &max_multisamples);
    if (self->multisample > max_multisamples) {
        log_warn(PRELOG"multisample > max_multisamples");
        self->multisample = max_multisamples;
    }

    glGenTextures(1, &visu->multisample_tex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, visu->multisample_tex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, self->multisample, GL_RGBA8, width, height, GL_FALSE);

    glGenRenderbuffers(1, &visu->multisample_color_buf);
    glBindRenderbuffer(GL_RENDERBUFFER, visu->multisample_color_buf);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, self->multisample, GL_RGBA8, width, height);

    glGenRenderbuffers(1, &visu->multisample_depth_buf);
    glBindRenderbuffer(GL_RENDERBUFFER, visu->multisample_depth_buf);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, self->multisample, GL_DEPTH_COMPONENT, width, height);

    glGenFramebuffers(1, &visu->multisample_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, visu->multisample_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, visu->multisample_tex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, visu->multisample_color_buf);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, visu->multisample_depth_buf);

    visu->multisample_active = true;
}

static void clear_multisample(VuVisu *self) {
    Visu *visu = self->impl;

    if (visu->multisample_tex != -1) {
        glDeleteTextures(1, &visu->multisample_tex);
        visu->multisample_tex = -1;
    }
    if (visu->multisample_color_buf != -1) {
        glDeleteRenderbuffers(1, &visu->multisample_color_buf);
        visu->multisample_color_buf = -1;
    }
    if (visu->multisample_depth_buf != -1) {
        glDeleteRenderbuffers(1, &visu->multisample_depth_buf);
        visu->multisample_depth_buf = -1;
    }
    if (visu->multisample_fbo != -1) {
        glDeleteFramebuffers(1, &visu->multisample_fbo);
        visu->multisample_fbo = -1;
    }

    visu->multisample_active = false;
}

static void gl_msg_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar *message, const void *userParam) {
    if (type == GL_DEBUG_TYPE_ERROR) {
        log_error(PRELOG"OpenGL: id = 0x%x, severity = 0x%x, message = %s", id, severity, message);
        abort();
    }

    GLuint ignored_ids[] = {
            0x20071, // buffer bound to VIDEO memory debug info

            // SIMD shader
            0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9,
            0xa, 0xb, 0xc, 0xd, 0xf,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
            0x1a, 0x1b // end {SIMD shader}
    };

    for (int i = 0; i < sizeof(ignored_ids) / sizeof(int); i++) {
        if (id == ignored_ids[i])
            return;
    }

    log_debug(PRELOG"OpenGL: type = 0x%x, id = 0x%x, severity = 0x%x, message = %s", type, id, severity, message);
}
