#include <pbase/mathc/float.h>
#include <pbase/mathc/utils/camera.h>
#include "visu/camera/orthogonal.h"

VuCamera *vu_orthogonal_camera_new(float z_near, float z_far) {
    // call super()
    VuCamera *self = vu_camera_new(z_near, z_far);

    // update virtual methods
    self->update_projection = vu_orthogonal_camera_update_projection;
    self->position_to_ray = vu_orthogonal_camera_position_to_ray;

    return self;
}

void vu_orthogonal_camera_update_projection(struct VuCamera *super, float wnd_width, float wnd_height) {
    // call super()
    vu_camera_update_projection(super, wnd_width, wnd_height);

    float width_half = wnd_width / (super->scale * 2);
    float height_half = wnd_height / (super->scale * 2);
    super->projection = mat4_camera_ortho(-width_half, width_half, -height_half, height_half, super->z_near,
                                          super->z_far);
}

void vu_orthogonal_camera_position_to_ray(const struct VuCamera *super,
                                          vec3 *out_pos, vec3 *out_dir,
                                          float wnd_x, float wnd_y) {
    float x = wnd_x - super->wnd_width_ * 0.5f;
    float y = super->wnd_height_ * 0.5f - wnd_y;

    vec4 pos = {{
                        x / super->scale,
                        y / super->scale,
                        0,
                        1
                }};

    pos = mat4_mul_vec(super->pose, pos);
    *out_pos = pos.xyz;
    *out_dir = vec3_neg(super->pose.col[2].xyz);     // -Z
}
