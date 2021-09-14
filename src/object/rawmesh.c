#include <pbase/rhc/log.h>
#include "visu/object/mesh.h"
#include "objectbase.h"

#define PRELOG "[RawMesh] "

static const char *vertex_code =
        "#version 330\n"
        "layout(location = 0) in vec4 position;"
        "layout(location = 1) in vec4 normal;"
        "out vec4 res_normal;"
        "uniform mat4 mvp;"
        "uniform mat4 mv;"
        "void main() {"
        "  gl_Position = mvp * position;"
        "  res_normal = mv * normal;"
        "}";

static const char *fragment_code =
        "#version 330\n"
        "in vec4 res_normal;"
        "uniform vec4 color;"
        "uniform vec3 light_source;"
        "uniform vec3 light_color;"
        "uniform float light_power;"
        "uniform vec3 ambient_color;"
        "void main() {"
        "  vec3 l = normalize(light_source);"
        "  vec3 ambient = ambient_color * vec3(color);"
        "  vec3 n = vec3(res_normal);"

        "  float cos_theta = clamp(dot( n, l ), 0, 1);"
        "  gl_FragColor = vec4(ambient + vec3(color) * light_color * light_power * cos_theta, color.a);"
        "}";

static const int position_location = 0;
static const int normal_location = 1;

static VuShaderProgram program;


typedef struct Impl {
    ObjectBase super;

    GLuint color_uniform;
    VuLightOptionUniforms light_option_uniforms;

    GLuint points_buffer;
    GLuint normals_buffer;
    GLuint points_size;
} Impl;


static void unload(VuRenderObject *super);

static void kill_impl(VuRenderObject *super);

static bool update(VuRenderObject *super);

static void render(VuRenderObject *super);


VuRenderObject *vu_raw_mesh_new(VuVisu *visu,
                                pCloud points,
                                pCloud normals,
                                vec4 color) {
    if (program.usages <= 0)
        program = vu_shader_program_new(vertex_code, fragment_code);

    // public
    VuRawMesh *self = p_rhc_calloc(sizeof *self);
    vu_render_object_init(&self->super, visu);

    self->points = points;
    self->normals = normals;
    self->color = color;
    self->wire_color = color;
    self->wire_size = 1;
    self->mesh_mode = VU_MESH_MODE_SURFACES;


    // private
    Impl *impl = p_rhc_calloc(sizeof *impl);
    self->super.impl = impl;

    object_base_init(&impl->super, visu, "RawMesh", &program, unload, kill_impl, update, render);

    object_base_add_property(&impl->super, "color", "vec4", self->color.v);
    object_base_add_property(&impl->super, "wire_color", "vec4", self->wire_color.v);
    object_base_add_property(&impl->super, "wire_size", "float", &self->wire_size);
    object_base_add_property(&impl->super, "mesh_mode", "enum vu_meshmode", &self->mesh_mode);

    object_base_add_light_option_properties_and_init(&impl->super, &self->light_options);

    return (VuRenderObject *) self;
}

static void unload(VuRenderObject *super) {
    Impl *impl = super->impl;
    glDeleteBuffers(1, &impl->points_buffer);
    glDeleteBuffers(1, &impl->normals_buffer);
    impl->points_buffer = 0;
    impl->normals_buffer = 0;
    super->update = true;
}

static void kill_impl(VuRenderObject *super) {
    VuRawMesh *self = (VuRawMesh *) super;

    if (super->kill_user_data) {
        p_cloud_kill(&self->points);
        p_cloud_kill(&self->normals);
    }

    p_rhc_free(super->impl);
}

static bool update(VuRenderObject *super) {
    Impl *impl = super->impl;
    VuRawMesh *self = (VuRawMesh *) super;

    if (self->points.size != self->normals.size) {
        log_error(PRELOG"points and normals clouds have different sizes, ignoring the update");
        return false;
    }

    impl->color_uniform = glGetUniformLocation(program.program, "color");
    vu_light_option_uniforms_find(&impl->light_option_uniforms, program.program);

    impl->points_size = self->points.size;

    if (impl->points_buffer == 0)
        glGenBuffers(1, &impl->points_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, impl->points_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 impl->points_size * sizeof(vec4),
                 self->points.data,
                 GL_STATIC_DRAW);

    if (impl->normals_buffer == 0)
        glGenBuffers(1, &impl->normals_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, impl->normals_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 impl->points_size * sizeof(vec4),
                 self->normals.data,
                 GL_STATIC_DRAW);

    return true;
}

static void render(VuRenderObject *super) {
    Impl *impl = super->impl;
    VuRawMesh *self = (VuRawMesh *) super;

    vu_light_option_uniforms_set(impl->light_option_uniforms, program.program, self->light_options);

    glBindBuffer(GL_ARRAY_BUFFER, impl->points_buffer);
    glEnableVertexAttribArray(position_location);
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, impl->normals_buffer);
    glEnableVertexAttribArray(normal_location);
    glVertexAttribPointer(normal_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    // render surface
    if (self->mesh_mode == VU_MESH_MODE_SURFACES || self->mesh_mode == VU_MESH_MODE_SURFACES_AND_WIREFRAME) {
        glUniform4fv(impl->color_uniform, 1, self->color.v);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, impl->points_size);
    }

    // render wireframe
    if (self->mesh_mode == VU_MESH_MODE_WIREFRAME || self->mesh_mode == VU_MESH_MODE_SURFACES_AND_WIREFRAME) {
        glUniform4fv(impl->color_uniform, 1, self->wire_color.v);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(self->wire_size);
        glDrawArrays(GL_TRIANGLES, 0, impl->points_size);
    }
}

