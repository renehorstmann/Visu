#include <pbase/rhc/allocator.h>
#include <pbase/mathc/float.h>
#include <pbase/mathc/utils/rotation.h>

#include "visu/control/mouserotate.h"


VuControl *vu_mouse_rotate_control_new() {
    VuMouseRotateControl *self = p_rhc_calloc(sizeof *self);

    // set interface (virtual methods)
    VuControl super;
    super.key_event = vu_mouse_rotate_control_key_event;
    super.mouse_click_event = vu_mouse_rotate_control_mouse_click_event;
    super.mouse_move_event = vu_mouse_rotate_control_mouse_move_event;
    super.mouse_scroll_event = vu_mouse_rotate_control_mouse_scroll_event;
    super.simulate = vu_mouse_rotate_control_simulate;
    super.set_pose = vu_mouse_rotate_control_set_pose;
    super.get_pose = vu_mouse_rotate_control_get_pose;
    self->super = super;

    self->center_pose = VU_MOUSE_ROTATE_CONTROL_INIT_CENTER_POSE;
    self->camera_distance = VU_MOUSE_ROTATE_CONTROL_INIT_CAMERA_DISTANCE;
    self->mouse_move_pos_x = self->mouse_move_pos_y = -1;
    self->mouse_rot_pos_x = self->mouse_rot_pos_y = -1;
    self->scroll = 1;

    return (VuControl *) self;
}

void vu_mouse_rotate_control_key_event(VuControl *super, char key, bool pressed) {
    // pass
}

void vu_mouse_rotate_control_mouse_click_event(VuControl *super,
                                               float wnd_x, float wnd_y, int button, bool clicked) {
    VuMouseRotateControl *self = (VuMouseRotateControl *) super;

    if (button == VU_CONTROL_MOUSE_BUTTON_LEFT) {
        if (clicked) {
            self->mouse_rot_pos_x = wnd_x;
            self->mouse_rot_pos_y = wnd_y;
        } else {
            self->mouse_rot_pos_x = -1;
            self->speed_rot_x = self->speed_rot_y = 0;
        }
    }
    if (button == VU_CONTROL_MOUSE_BUTTON_MIDDLE) {
        if (clicked) {
            self->mouse_move_pos_x = wnd_x;
            self->mouse_move_pos_y = wnd_y;
        } else {
            self->mouse_move_pos_x = -1;
            self->speed_x = self->speed_y = 0;
        }
    }
}

void vu_mouse_rotate_control_mouse_move_event(VuControl *super, float wnd_x, float wnd_y) {
    VuMouseRotateControl *self = (VuMouseRotateControl *) super;

    if (self->mouse_rot_pos_x >= 0) {
        float x_speed = (self->mouse_rot_pos_x - wnd_x) / VU_MOUSE_ROTATE_CONTROL_MAX_SPEED_PIXELS;
        float y_speed = (self->mouse_rot_pos_y - wnd_y) / VU_MOUSE_ROTATE_CONTROL_MAX_SPEED_PIXELS;

        // speed in range [-1 : 1]
        self->speed_rot_y = VU_MOUSE_ROTATE_CONTROL_XY_ROTATE_SPEED * fmaxf(-1, fminf(1, x_speed));
        self->speed_rot_x = VU_MOUSE_ROTATE_CONTROL_XY_ROTATE_SPEED * fmaxf(-1, fminf(1, y_speed));
    }
    if (self->mouse_move_pos_x >= 0) {
        float x_speed = (self->mouse_move_pos_x - wnd_x) / VU_MOUSE_ROTATE_CONTROL_MAX_SPEED_PIXELS;
        float y_speed = (self->mouse_move_pos_y - wnd_y) / VU_MOUSE_ROTATE_CONTROL_MAX_SPEED_PIXELS;

        // speed in range [-1 : 1] (flip y...)
        self->speed_x = VU_MOUSE_ROTATE_CONTROL_XY_MOVE_SPEED * fmaxf(-1, fminf(1, x_speed));
        self->speed_y = VU_MOUSE_ROTATE_CONTROL_XY_MOVE_SPEED * fmaxf(-1, fminf(1, -y_speed));
    }
}

void vu_mouse_rotate_control_mouse_scroll_event(VuControl *super, bool up) {
    VuMouseRotateControl *self = (VuMouseRotateControl *) super;

    if (up)
        self->scroll *= VU_MOUSE_ROTATE_CONTROL_SCROLL_FACTOR;
    else
        self->scroll /= VU_MOUSE_ROTATE_CONTROL_SCROLL_FACTOR;
}

void vu_mouse_rotate_control_simulate(VuControl *super, VuCamera *camera, double delta_time) {
    VuMouseRotateControl *self = (VuMouseRotateControl *) super;

    mat4 center_pose = self->center_pose;
    mat3 center_pose_rot = mat4_get_upper_left3(center_pose);

    if (self->speed_x != 0) {
        vec3 v_trans = center_pose.col[3].xyz;
        vec3 speed = {{self->speed_x / self->scroll * (float) delta_time, 0, 0}};
        vec3 add = mat3_mul_vec(center_pose_rot, speed);
        center_pose.col[3].xyz = vec3_add_vec(v_trans, add);
    }
    if (self->speed_y != 0) {
        vec3 v_trans = center_pose.col[3].xyz;
        vec3 speed = {{0, self->speed_y / self->scroll * (float) delta_time, 0}};
        vec3 add = mat3_mul_vec(center_pose_rot, speed);
        center_pose.col[3].xyz = vec3_add_vec(v_trans, add);
    }

    if (self->speed_rot_x != 0) {
        vec4 angle_axis;
        angle_axis.xyz = center_pose.col[0].xyz;
        angle_axis.w = self->speed_rot_x * (float) delta_time;
        mat3 rotation = mat3_rotation_from_angle_axis(angle_axis);
        center_pose_rot = mat3_mul_mat(rotation, center_pose_rot);
    }
    if (self->speed_rot_y != 0) {
        vec4 angle_axis;
        angle_axis.xyz = center_pose.col[1].xyz;
        angle_axis.w = self->speed_rot_y * (float) delta_time;
        mat3 rotation = mat3_rotation_from_angle_axis(angle_axis);
        center_pose_rot = mat3_mul_mat(rotation, center_pose_rot);
    }

    mat4_set_this_upper_left3(&center_pose, center_pose_rot);
    self->center_pose = center_pose;

    mat4_set_this_upper_left3(&camera->pose, center_pose_rot);

    for (int i = 0; i < 3; i++)
        camera->pose.v[3 * 4 + i] = self->center_pose.v[3 * 4 + i]
                                    + self->center_pose.v[2 * 4 + i] * self->camera_distance;

    camera->scale = self->scroll;
}


void vu_mouse_rotate_control_set_pose(VuControl *super, mat4 pose) {
    VuMouseRotateControl *self = (VuMouseRotateControl *) super;
    self->center_pose = pose;
}

mat4 vu_mouse_rotate_control_get_pose(const VuControl *super) {
    VuMouseRotateControl *self = (VuMouseRotateControl *) super;
    return self->center_pose;
}
