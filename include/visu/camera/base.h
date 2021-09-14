#ifndef VISU_CAMERA_BASE_H
#define VISU_CAMERA_BASE_H


#include <pbase/mathc/types/float.h>

struct VuCamera;

typedef void (*vu_camera_update_projection_fun)(struct VuCamera *self, float wnd_width, float wnd_height);

typedef void (*vu_camera_position_to_ray_fun)(const struct VuCamera *self, vec3 *out_pos, vec3 *out_dir,
                                              float wnd_x, float wnd_y);

/**
 * Camera super class that implements pose, projection and v-methods to update them.
 * Implementations are f. e.: VuPerspectiveCamera_s, VuOrthogonalCamera.
 * The pose can be set by user or a Camera controller.
 */
typedef struct VuCamera {
    mat4 pose;
    mat4 projection;
    float z_near, z_far;
    float scale;

    float wnd_width_, wnd_height_;

    /** call this method to update the projection matrix */
    vu_camera_update_projection_fun update_projection;

    /** converts a f. e. a mouse click into a 3d ray */
    vu_camera_position_to_ray_fun position_to_ray;
} VuCamera;

/**
 * Creates a new camera super class.
 * @param z_near: near clipping plane
 * @param z_far: far clipping plane
 */
VuCamera *vu_camera_new(float z_near, float z_far);

void vu_camera_kill(VuCamera **self_ptr);

/** The super class projection is always eye. This super method sets the inner wnd_w/h_ to the actual */
void vu_camera_update_projection(struct VuCamera *self, float wnd_width, float wnd_height);

/** The super class method that works with the eye projection */
void vu_camera_position_to_ray(const struct VuCamera *self, vec3 *out_pos, vec3 *out_dir, float wnd_x, float wnd_y);


#endif //VISU_CAMERA_BASE_H
