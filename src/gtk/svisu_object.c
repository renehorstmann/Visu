#include "visu/gtk/svisu_object.h"

VuPoints *svisu_points(SVisu *self, pCloud points, vec4 color, float point_size) {
    VuRenderObject *ro = vu_points_new(self->visu, points, color, point_size);
    svisu_append_render_object(self, ro);
    return (VuPoints *) ro;
}

VuPointsColored *svisu_points_colored(SVisu *self, pCloud points, pCloud colors, float point_size) {
    VuRenderObject *ro = vu_points_colored_new(self->visu, points, colors, point_size);
    svisu_append_render_object(self, ro);
    return (VuPointsColored *) ro;
}

VuPointNormals *svisu_point_normals(SVisu *self, pCloud points, pCloud normals,
                                    vec4 color, float point_size) {
    VuRenderObject *ro = vu_point_normals_new(self->visu, points, normals, color, point_size);
    svisu_append_render_object(self, ro);
    return (VuPointNormals *) ro;
}

VuPointNormalsColored *svisu_point_normals_colored(SVisu *self, pCloud points, pCloud normals,
                                                   pCloud colors, float point_size) {
    VuRenderObject *ro = vu_point_normals_colored_new(self->visu, points, normals, colors, point_size);
    svisu_append_render_object(self, ro);
    return (VuPointNormalsColored *) ro;
}


VuLines *svisu_lines(SVisu *self, pCloud points, vec4 color, float line_size, enum vu_linemode line_mode) {
    VuRenderObject *ro = vu_lines_new(self->visu, points, color, line_size, line_mode);
    svisu_append_render_object(self, ro);
    return (VuLines *) ro;
}

VuLinesColored *svisu_lines_colored(SVisu *self, pCloud points, pCloud colors,
                                    float line_size, enum vu_linemode line_mode) {
    VuRenderObject *ro = vu_lines_colored_new(self->visu, points, colors, line_size, line_mode);
    svisu_append_render_object(self, ro);
    return (VuLinesColored *) ro;
}

VuMesh *svisu_mesh(SVisu *self, pCloud points, pMeshIndices indices, vec4 color) {
    VuRenderObject *ro = vu_mesh_new(self->visu, points, indices, color);
    svisu_append_render_object(self, ro);
    return (VuMesh *) ro;
}

VuMeshColored *svisu_mesh_colored(SVisu *self, pCloud points, pCloud colors, pMeshIndices indices) {
    VuRenderObject *ro = vu_mesh_colored_new(self->visu, points, colors, indices);
    svisu_append_render_object(self, ro);
    return (VuMeshColored *) ro;
}

VuMeshColoredMatt *svisu_mesh_colored_matt(SVisu *self, pCloud points, pCloud colors, pMeshIndices indices) {
    VuRenderObject *ro = vu_mesh_colored_matt_new(self->visu, points, colors, indices);
    svisu_append_render_object(self, ro);
    return (VuMeshColoredMatt *) ro;
}

VuRawMesh *svisu_raw_mesh(SVisu *self, pCloud points, pCloud normals, vec4 color) {
    VuRenderObject *ro = vu_raw_mesh_new(self->visu, points, normals, color);
    svisu_append_render_object(self, ro);
    return (VuRawMesh *) ro;
}

VuRawMeshColored *svisu_raw_mesh_colored(SVisu *self, pCloud points, pCloud normals, pCloud colors) {
    VuRenderObject *ro = vu_raw_mesh_colored_new(self->visu, points, normals, colors);
    svisu_append_render_object(self, ro);
    return (VuRawMeshColored *) ro;
}
