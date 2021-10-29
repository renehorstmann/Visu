#include <pbase/rhc/alloc.h>
#include <pbase/mathc/float.h>
#include <pbase/mathc/utils/camera.h>
#include "visu/camera/perspective.h"

VuCamera *vu_perspective_camera_new(float z_near, float z_far) {
    VuPerspectiveCamera *self = p_rhc_calloc(sizeof *self);
    // call super() and copy camera
    VuCamera *base = vu_camera_new(z_near, z_far);
    self->super = *base;
    vu_camera_kill(&base);

    // update virtual methods
    self->super.update_projection = vu_perspective_camera_update_projection;
    self->super.position_to_ray = vu_perspective_camera_position_to_ray;

    // init members
    self->aperture_angle = (float) (M_PI / 8);

    return (VuCamera *) self;
}

void vu_perspective_camera_update_projection(struct VuCamera *super, float wnd_width, float wnd_height) {
    // call super()
    vu_camera_update_projection(super, wnd_width, wnd_height);

    // cast to child class
    VuPerspectiveCamera *self = (VuPerspectiveCamera *) super;

    super->projection = mat4_camera_perspective(super->scale * 2 * self->aperture_angle,
                                                wnd_width / wnd_height,
                                                super->z_near, super->z_far);
}

void vu_perspective_camera_position_to_ray(const struct VuCamera *super,
                                           vec3 *out_pos, vec3 *out_dir,
                                           float wnd_x, float wnd_y) {
    // cast to child class
    VuPerspectiveCamera *self = (VuPerspectiveCamera *) super;

    // convert window x, y to camera coordinates (center is 0)
    float x = 2 * wnd_x / super->wnd_width_ - 1;
    float y = 2 * wnd_y / super->wnd_height_ - 1;

    vec3 view = vec3_neg(super->pose.col[2].xyz); // -Z
    vec3 v = super->pose.col[1].xyz;              // Y
    vec3 h = vec3_neg(super->pose.col[0].xyz);    // -X

    float fov = tanf(super->scale * self->aperture_angle);

    float v_len = fov * super->z_near;
    float h_len = v_len * super->wnd_width_ / super->wnd_height_;


    for (int i = 0; i < 3; i++)
        out_pos->v[i] = super->pose.m[3][i] + view.v[i] * super->z_near - h.v[i] * h_len * x - v.v[i] * v_len * y;

    *out_dir = vec3_sub_vec(*out_pos, super->pose.col[3].xyz);
    *out_dir = vec3_normalize(*out_dir);
}
