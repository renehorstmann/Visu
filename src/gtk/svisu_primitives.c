#include <pbase/mathc/float.h>
#include <pbase/meshprimitives.h>

#include "visu/gtk/svisu_object.h"
#include "visu/gtk/svisu_primitives.h"

void svisu_coords(SVisu *self, mat4 pose, float length,
                  vec4 color_x, vec4 color_y, vec4 color_z) {

    vec3 x_extend = vec3_scale(pose.col[0].xyz, length);
    vec3 y_extend = vec3_scale(pose.col[1].xyz, length);
    vec3 z_extend = vec3_scale(pose.col[2].xyz, length);

    vec3 x_sphere_pos = vec3_add_vec(
            pose.col[3].xyz,
            vec3_scale(pose.col[0].xyz, length / 3));

    vec3 start_point = pose.col[3].xyz;

    float radius = length / 20;
    float sphere_radius = radius * 1.5f;
    float vertex_density = radius / 2;

    pCloud x_points, x_normals;
    pCloud y_points, y_normals;
    pCloud z_points, z_normals;
    pCloud x_sphere_points, x_sphere_normals;

    p_meshprimitives_raw_arrow_swapped(&x_points, &x_normals, start_point, x_extend, radius, vertex_density);
    p_meshprimitives_raw_arrow_swapped(&y_points, &y_normals, start_point, y_extend, radius, vertex_density);
    p_meshprimitives_raw_arrow_swapped(&z_points, &z_normals, start_point, z_extend, radius, vertex_density);
    p_meshprimitives_raw_sphere(&x_sphere_points, &x_sphere_normals, x_sphere_pos, sphere_radius, vertex_density);

    VuRawMesh *x_ro = svisu_raw_mesh(self, x_points, x_normals, color_x);
    VuRawMesh *y_ro = svisu_raw_mesh(self, y_points, y_normals, color_y);
    VuRawMesh *z_ro = svisu_raw_mesh(self, z_points, z_normals, color_z);
    VuRawMesh *x_sphere_ro = svisu_raw_mesh(self, x_sphere_points, x_sphere_normals, color_x);

    // Cloud kill by visu
    x_ro->super.kill_user_data = true;
    y_ro->super.kill_user_data = true;
    z_ro->super.kill_user_data = true;
    x_sphere_ro->super.kill_user_data = true;
}

VuRawMesh *svisu_plane(SVisu *self, mat4 pose, float width, float height, vec4 color) {
    pCloud points, normals;
    p_meshprimitives_raw_plane(&points, &normals, pose, width, height);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}

VuRawMesh *svisu_box(SVisu *self, mat4 pose, float width, float height, float extend, vec4 color) {
    pCloud points, normals;
    p_meshprimitives_raw_box(&points, &normals, pose, width, height, extend);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}

VuRawMesh *svisu_circle(SVisu *self, mat4 pose, float radius, vec4 color) {
    float vertex_density = radius / 4;
    pCloud points, normals;
    p_meshprimitives_raw_circle(&points, &normals, pose, radius, vertex_density);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}

VuRawMesh *svisu_cylinder_hull(SVisu *self, vec3 start_point, vec3 extend,
                               float radius, vec4 color) {
    float vertex_density = radius / 4;
    pCloud points, normals;
    p_meshprimitives_raw_cylinder_hull(&points, &normals, start_point, extend, radius, vertex_density);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}

VuRawMesh *svisu_cylinder(SVisu *self, vec3 start_point, vec3 extend,
                          float radius, vec4 color) {
    float vertex_density = radius / 4;
    pCloud points, normals;
    p_meshprimitives_raw_cylinder(&points, &normals, start_point, extend, radius, vertex_density);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}

VuRawMesh *svisu_cone_part_hull(SVisu *self, vec3 start_point, vec3 extend,
                                float start_radius, float end_radius, vec4 color) {
    float vertex_density = (start_radius > end_radius ? start_radius : end_radius) / 4;
    pCloud points, normals;
    p_meshprimitives_raw_cone_part_hull(&points, &normals, start_point, extend, start_radius, end_radius,
                                        vertex_density);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}

VuRawMesh *svisu_cone_hull(SVisu *self, vec3 apex_point, vec3 extend,
                           float opening_angle_rad, vec4 color) {
    float length = vec3_norm(extend);
    float radius = length * tanf(opening_angle_rad);
    float vertex_density = radius / 4;
    pCloud points, normals;
    p_meshprimitives_raw_cone_hull(&points, &normals, apex_point, extend, opening_angle_rad, vertex_density);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}

VuRawMesh *svisu_cone(SVisu *self, vec3 apex_point, vec3 extend,
                      float opening_angle_rad, vec4 color) {
    float length = vec3_norm(extend);
    float radius = length * tanf(opening_angle_rad);
    float vertex_density = radius / 4;
    pCloud points, normals;
    p_meshprimitives_raw_cone(&points, &normals, apex_point, extend, opening_angle_rad, vertex_density);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}

VuRawMesh *svisu_sphere(SVisu *self, vec3 mid_pos, float radius, vec4 color) {
    float vertex_density = radius / 4;
    pCloud points, normals;
    p_meshprimitives_raw_sphere(&points, &normals, mid_pos, radius, vertex_density);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}

VuRawMesh *svisu_half_sphere(SVisu *self, mat4 pose, float radius, vec4 color) {
    float vertex_density = radius / 4;
    pCloud points, normals;
    p_meshprimitives_raw_half_sphere(&points, &normals, pose, radius, vertex_density);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}

VuRawMesh *svisu_arrow(SVisu *self, vec3 apex_point, vec3 extend,
                       float radius, vec4 color) {
    float vertex_density = radius / 4;
    pCloud points, normals;
    p_meshprimitives_raw_arrow(&points, &normals, apex_point, extend, radius, vertex_density);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}

VuRawMesh *svisu_arrow_swapped(SVisu *self, vec3 end_point, vec3 extend,
                               float radius, vec4 color) {
    float vertex_density = radius / 4;
    pCloud points, normals;
    p_meshprimitives_raw_arrow_swapped(&points, &normals, end_point, extend, radius, vertex_density);
    VuRawMesh *ro = svisu_raw_mesh(self, points, normals, color);
    ro->super.kill_user_data = true; // Cloud kill by visu
    return ro;
}
