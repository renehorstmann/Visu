#ifndef VISU_GTK_SVISU_PRIMITIVES_H
#define VISU_GTK_SVISU_PRIMITIVES_H


#include "svisu_base.h"
#include "visu/object/mesh.h"


/**
 * Shows the coordination system of a pose.
 */
void svisu_coords(SVisu *self, mat4 pose, float length,
                  vec4 color_x, vec4 color_y, vec4 color_z);



/** Shows a plane in svisu */
VuRawMesh *svisu_plane(SVisu *self, mat4 pose, float width, float height, vec4 color);

/** Shows a box in svisu */
VuRawMesh *svisu_box(SVisu *self, mat4 pose, float width, float height, float extend, vec4 color);

/** Shows a circle in svisu */
VuRawMesh *svisu_circle(SVisu *self, mat4 pose, float radius, vec4 color);

/** Shows a cylinder hull in svisu */
VuRawMesh *svisu_cylinder_hull(SVisu *self, vec3 start_point, vec3 extend,
                               float radius, vec4 color);

/** Shows a cylinder in svisu */
VuRawMesh *svisu_cylinder(SVisu *self, vec3 start_point, vec3 extend,
                          float radius, vec4 color);

/** Shows a cone part hull in svisu */
VuRawMesh *svisu_cone_part_hull(SVisu *self, vec3 start_point, vec3 extend,
                                float start_radius, float end_radius, vec4 color);

/** Shows a cone hull in svisu */
VuRawMesh *svisu_cone_hull(SVisu *self, vec3 apex_point, vec3 extend,
                           float opening_angle_rad, vec4 color);

/** Shows a cone in svisu */
VuRawMesh *svisu_cone(SVisu *self, vec3 apex_point, vec3 extend,
                      float opening_angle_rad, vec4 color);

/** Shows a sphere in svisu */
VuRawMesh *svisu_sphere(SVisu *self, vec3 mid_pos, float radius, vec4 color);

/** Shows a half sphere in svisu */
VuRawMesh *svisu_half_sphere(SVisu *self, mat4 pose, float radius, vec4 color);

/** Shows an arrow in svisu */
VuRawMesh *svisu_arrow(SVisu *self, vec3 apex_point, vec3 extend,
                       float radius, vec4 color);

/** Shows an arrow in svisu */
VuRawMesh *svisu_arrow_swapped(SVisu *self, vec3 end_point, vec3 extend,
                               float radius, vec4 color);



#endif //VISU_GTK_SVISU_PRIMITIVES_H
