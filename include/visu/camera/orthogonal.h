#ifndef VISU_CAMERA_ORTHOGONAL_H
#define VISU_CAMERA_ORTHOGONAL_H


#include "base.h"

/**
 * Creates a new orthogonal camera.
 * @param z_near: near clipping plane
 * @param z_far: far clipping plane
 */
VuCamera *vu_orthogonal_camera_new(float z_near, float z_far);

/** Sets the projection for an orthogonal camera */
void vu_orthogonal_camera_update_projection(struct VuCamera *super, float wnd_width, float wnd_height);

/** Calculates the ray (from a window point) for the orthogonal camera */
void vu_orthogonal_camera_position_to_ray(const struct VuCamera *super,
                                          vec3 *out_pos, vec3 *out_dir,
                                          float wnd_x, float wnd_y);


#endif //VISU_CAMERA_ORTHOGONAL_H
