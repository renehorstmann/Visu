#include "visu/object/lines.h"
#include "objectbase.h"

static const char *vertex_code =
        "#version 330\n"
        "layout(location = 0) in vec4 position;"
        "uniform mat4 mvp;"
        "void main() {"
        "  gl_Position = mvp * position;"
        "}";

static const char *fragment_code =
        "#version 330 \n"
        "uniform vec4 color;"
        "void main() {"
        "  gl_FragColor = color;"
        "}";

static const int position_location = 0;

static VuShaderProgram program;


typedef struct Impl {
    ObjectBase super;

    GLuint color_uniform;

    GLuint points_buffer;
    GLuint points_size;
} Impl;


static void unload(VuRenderObject *super);

static void kill_impl(VuRenderObject *super);

static bool update(VuRenderObject *super);

static void render(VuRenderObject *super);


VuRenderObject *vu_lines_new(VuVisu *visu,
                             pCloud points,
                             vec4 color,
                             float line_size,
                             enum vu_linemode line_mode) {
    if (program.usages <= 0)
        program = vu_shader_program_new(vertex_code, fragment_code);

    // public
    VuLines *self = p_rhc_calloc(sizeof *self);
    vu_render_object_init(&self->super, visu);

    self->points = points;
    self->color = color;
    self->line_size = line_size;
    self->line_mode = line_mode;

    // private
    Impl *impl = p_rhc_calloc(sizeof *impl);
    self->super.impl = impl;

    object_base_init(&impl->super, visu, "Lines", &program, unload, kill_impl, update, render);

    object_base_add_property(&impl->super, "color", "vec4", self->color.v);
    object_base_add_property(&impl->super, "line_size", "float", &self->line_size);
    object_base_add_property(&impl->super, "line_mode", "enum vu_linemode", &self->line_mode);

    return (VuRenderObject *) self;
}

static void unload(VuRenderObject *super) {
    Impl *impl = super->impl;
    glDeleteBuffers(1, &impl->points_buffer);
    impl->points_buffer = 0;
    super->update = true;
}

static void kill_impl(VuRenderObject *super) {
    VuLines *self = (VuLines *) super;

    if (super->kill_user_data) {
        p_cloud_kill(&self->points);
    }

    p_rhc_free(super->impl);
}

static bool update(VuRenderObject *super) {
    Impl *impl = super->impl;
    VuLines *self = (VuLines *) super;

    impl->color_uniform = glGetUniformLocation(program.program, "color");

    impl->points_size = self->points.size;

    if (impl->points_buffer == 0)
        glGenBuffers(1, &impl->points_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, impl->points_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 impl->points_size * sizeof(vec4),
                 self->points.data,
                 GL_STATIC_DRAW);

    return true;
}

static void render(VuRenderObject *super) {
    Impl *impl = super->impl;
    VuLines *self = (VuLines *) super;

    glBindBuffer(GL_ARRAY_BUFFER, impl->points_buffer);
    glEnableVertexAttribArray(position_location);
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glUniform4fv(impl->color_uniform, 1, self->color.v);

    glLineWidth(self->line_size);
    GLenum mode = (GLenum[]) {GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP}[self->line_mode];
    glDrawArrays(mode, 0, impl->points_size);
}

