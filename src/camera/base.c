#include <pbase/rhc/alloc.h>
#include <pbase/mathc/float.h>

#include "visu/camera/base.h"

VuCamera *vu_camera_new(float z_near, float z_far) {
    VuCamera *self = p_rhc_calloc(sizeof *self);
    // virtual methods:
    self->update_projection = vu_camera_update_projection;
    self->position_to_ray = vu_camera_position_to_ray;

    // init members
    self->pose = mat4_eye();
    self->projection = mat4_eye();
    self->z_near = z_near;
    self->z_far = z_far;
    self->scale = 1;
    self->wnd_width_ = self->wnd_height_ = 0;
    return self;
}

void vu_camera_kill(VuCamera **self_ptr) {
    p_rhc_free(*self_ptr);
    *self_ptr = NULL;
}

void vu_camera_update_projection(struct VuCamera *self, float wnd_width, float wnd_height) {
    self->wnd_width_ = wnd_width;
    self->wnd_height_ = wnd_height;
}

void vu_camera_position_to_ray(const struct VuCamera *self, vec3 *out_pos, vec3 *out_dir, float wnd_x, float wnd_y) {
    vec4 pos = {{wnd_x, wnd_y, 0, 1}};
    pos = mat4_mul_vec(self->pose, pos);
    *out_pos = pos.xyz;
    *out_dir = self->pose.col[2].xyz;    // Z
}
