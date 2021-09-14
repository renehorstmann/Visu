#include <pbase/rhc/log.h>
#include "visu/object/points.h"
#include "objectbase.h"

#define PRELOG "[Visu::PointsColored] "

static const char *vertex_code =
        "#version 330\n"
        "layout(location = 0) in vec4 position;"
        "layout(location = 1) in vec4 color;"
        "out vec4 frag_color;"
        "uniform mat4 mvp;"
        "void main() {"
        "  gl_Position = mvp * position;"
        "  frag_color = color;"
        "}";

static const char *fragment_code =
        "#version 330 \n"
        "in vec4 frag_color;"
        "void main() {"
        "  float r = 0.0;"
        "  vec2 cxy = 2.0 * gl_PointCoord - 1.0;"
        "  r = dot(cxy, cxy);"
        "  if (r > 1.0) {"
        "    discard;"
        "  }"
        "  gl_FragColor = frag_color;"
        "}";

static const int position_location = 0;
static const int color_location = 1;

static VuShaderProgram program;


typedef struct Impl {
    ObjectBase super;

    GLuint points_buffer;
    GLuint colors_buffer;
    GLuint points_size;
} Impl;

static void unload(VuRenderObject *super);

static void kill_impl(VuRenderObject *super);

static bool update(VuRenderObject *super);

static void render(VuRenderObject *super);


VuRenderObject *vu_points_colored_new(VuVisu *visu,
                                      pCloud points,
                                      pCloud colors,
                                      float point_size) {
    if (program.usages <= 0)
        program = vu_shader_program_new(vertex_code, fragment_code);

    // public
    VuPointsColored *self = p_rhc_calloc(sizeof *self);
    vu_render_object_init(&self->super, visu);

    self->points = points;
    self->colors = colors;
    self->point_size = point_size;


    // private
    Impl *impl = p_rhc_calloc(sizeof *impl);
    self->super.impl = impl;

    object_base_init(&impl->super, visu, "PointsColored", &program, unload, kill_impl, update, render);

    object_base_add_property(&impl->super, "point_size", "float", &self->point_size);

    return (VuRenderObject *) self;
}


static void unload(VuRenderObject *super) {
    Impl *impl = super->impl;
    glDeleteBuffers(1, &impl->points_buffer);
    glDeleteBuffers(1, &impl->colors_buffer);
    impl->points_buffer = 0;
    impl->colors_buffer = 0;
    super->update = true;
}

static void kill_impl(VuRenderObject *super) {
    VuPointsColored *self = (VuPointsColored *) super;

    if (super->kill_user_data) {
        p_cloud_kill(&self->points);
        p_cloud_kill(&self->colors);
    }

    p_rhc_free(super->impl);
}

static bool update(VuRenderObject *super) {
    Impl *impl = super->impl;
    VuPointsColored *self = (VuPointsColored *) super;

    if (self->points.size != self->colors.size) {
        log_error(PRELOG"points and colors clouds have different sizes, ignoring the update");
        return false;
    }

    impl->points_size = self->points.size;

    if (impl->points_buffer == 0)
        glGenBuffers(1, &impl->points_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, impl->points_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 impl->points_size * sizeof(vec4),
                 self->points.data,
                 GL_STATIC_DRAW);

    if (impl->colors_buffer == 0)
        glGenBuffers(1, &impl->colors_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, impl->colors_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 impl->points_size * sizeof(vec4),
                 self->colors.data,
                 GL_STATIC_DRAW);

    return true;
}

static void render(VuRenderObject *super) {
    Impl *impl = super->impl;
    VuPointsColored *self = (VuPointsColored *) super;

    glBindBuffer(GL_ARRAY_BUFFER, impl->points_buffer);
    glEnableVertexAttribArray(position_location);
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, impl->colors_buffer);
    glEnableVertexAttribArray(color_location);
    glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glPointSize(self->point_size);
    glDrawArrays(GL_POINTS, 0, impl->points_size);
}

