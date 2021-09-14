#ifndef VISU_CAMERA_PERSPECTIVE_H
#define VISU_CAMERA_PERSPECTIVE_H


#include "base.h"

/** Perspective implementation for the VuCamera */
typedef struct {
    VuCamera super;

    float aperture_angle;
} VuPerspectiveCamera;

/**
 * Creates a new perspective camera.
 * @param z_near: near clipping plane
 * @param z_far: far clipping plane
 */
VuCamera *vu_perspective_camera_new(float z_near, float z_far);

/** Sets the projection for an perspective camera */
void vu_perspective_camera_update_projection(struct VuCamera *super, float wnd_width, float wnd_height);

/** Calculates the ray (from a window point) for the perspective camera */
void vu_perspective_camera_position_to_ray(const struct VuCamera *super,
                                           vec3 *out_pos, vec3 *out_dir,
                                           float wnd_x, float wnd_y);

#endif //VISU_CAMERA_PERSPECTIVE_H
