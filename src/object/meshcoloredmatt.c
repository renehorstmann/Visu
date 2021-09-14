#include <pbase/rhc/log.h>
#include "visu/object/mesh.h"
#include "objectbase.h"
#include "mesh_operations.h"

#define PRELOG "[Visu::MeshColoredMatt] "

static const char *vertex_code =
        "#version 330\n"
        "layout(location = 0) in vec4 position;"
        "layout(location = 1) in vec4 color;"
        "out vec4 res_color;"
        "uniform mat4 mvp;"
        "void main() {"
        "  gl_Position = mvp * position;"
        "  res_color = color;"
        "}";

static const char *fragment_code =
        "#version 330\n"
        "in vec4 res_color;"
        "void main() {"
        "  gl_FragColor = res_color;"
        "}";

static const int position_location = 0;
static const int color_location = 1;

static VuShaderProgram program;


typedef struct Impl {
    ObjectBase super;

    GLuint points_buffer;
    GLuint colors_buffer;
    GLuint points_size;

    pCloud mesh_points;
    pCloud mesh_colors;
} Impl;

static void unload(VuRenderObject *super);

static void kill_impl(VuRenderObject *super);

static bool update(VuRenderObject *super);

static void render(VuRenderObject *super);


VuRenderObject *vu_mesh_colored_matt_new(VuVisu *visu,
                                         pCloud points,
                                         pCloud colors,
                                         pMeshIndices indices) {
    if (program.usages <= 0)
        program = vu_shader_program_new(vertex_code, fragment_code);

    // public
    VuMeshColoredMatt *self = p_rhc_calloc(sizeof *self);
    vu_render_object_init(&self->super, visu);

    self->points = points;
    self->colors = colors;
    self->indices = indices;
    self->wire_size = 1;
    self->mesh_mode = VU_MESH_MODE_SURFACES;


    // private
    Impl *impl = p_rhc_calloc(sizeof *impl);
    self->super.impl = impl;

    object_base_init(&impl->super, visu, "MeshColoredMatt", &program, unload, kill_impl, update, render);

    object_base_add_property(&impl->super, "wire_size", "float", &self->wire_size);
    object_base_add_property(&impl->super, "mesh_mode", "enum vu_meshmode", &self->mesh_mode);

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
    Impl *impl = super->impl;
    VuMeshColoredMatt *self = (VuMeshColoredMatt *) super;

    if (super->kill_user_data) {
        p_cloud_kill(&self->points);
        p_cloud_kill(&self->colors);
        p_mesh_indices_kill(&self->indices);
    }

    // multiple calls dont hurt (if update got an err)
    p_cloud_kill(&impl->mesh_points);
    p_cloud_kill(&impl->mesh_colors);

    p_rhc_free(super->impl);
}

static bool update(VuRenderObject *super) {
    Impl *impl = super->impl;
    VuMeshColoredMatt *self = (VuMeshColoredMatt *) super;

    // multiple calls dont hurt (if update got an err)
    p_cloud_kill(&impl->mesh_points);
    p_cloud_kill(&impl->mesh_colors);

    if (self->points.size != self->colors.size) {
        log_error(PRELOG"Ignoring update: points and colors clouds have different sizes");
        return false;
    }

    if (!p_mesh_indices_check_in_range(self->indices, 0, self->points.size)) {
        log_error("Ignoring update: mesh_indices are indexing none valid points: "
                  "point_size=%i", self->points.size);
        return false;
    }

    impl->mesh_points = vu_mesh_expand_cloud(self->points, self->indices);
    impl->mesh_colors = vu_mesh_expand_cloud(self->colors, self->indices);

    impl->points_size = impl->mesh_points.size;

    if (impl->points_buffer == 0)
        glGenBuffers(1, &impl->points_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, impl->points_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 impl->points_size * sizeof(vec4),
                 impl->mesh_points.data,
                 GL_STATIC_DRAW);

    if (impl->colors_buffer == 0)
        glGenBuffers(1, &impl->colors_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, impl->colors_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 impl->points_size * sizeof(vec4),
                 impl->mesh_colors.data,
                 GL_STATIC_DRAW);

    // kill the clouds, data was transferred to OpenGL
    p_cloud_kill(&impl->mesh_points);
    p_cloud_kill(&impl->mesh_colors);

    return true;
}

static void render(VuRenderObject *super) {
    Impl *impl = super->impl;
    VuMeshColoredMatt *self = (VuMeshColoredMatt *) super;

    glBindBuffer(GL_ARRAY_BUFFER, impl->points_buffer);
    glEnableVertexAttribArray(position_location);
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, impl->colors_buffer);
    glEnableVertexAttribArray(color_location);
    glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    // render surface
    if (self->mesh_mode == VU_MESH_MODE_SURFACES || self->mesh_mode == VU_MESH_MODE_SURFACES_AND_WIREFRAME) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, impl->points_size);
    }

    // render wireframe
    if (self->mesh_mode == VU_MESH_MODE_WIREFRAME || self->mesh_mode == VU_MESH_MODE_SURFACES_AND_WIREFRAME) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(self->wire_size);
        glDrawArrays(GL_TRIANGLES, 0, impl->points_size);
    }
}

