#include <pbase/rhc/allocator.h>
#include <pbase/rhc/log.h>
#include <pbase/mathc/float.h>
#include <pbase/mathc/utils/rotation.h>

#include "visu/control/keyflow.h"

#define PRELOG "[Visu::KeyFlowControl] "

VuControl *vu_key_flow_control_new() {
    VuKeyFlowControl *self = p_rhc_calloc(sizeof *self);

    // set interface (virtual methods)
    VuControl super = {0};
    super.key_event = vu_key_flow_control_key_event;
    super.mouse_click_event = vu_key_flow_control_mouse_click_event;
    super.mouse_move_event = vu_key_flow_control_mouse_move_event;
    super.mouse_scroll_event = vu_key_flow_control_mouse_scroll_event;
    super.simulate = vu_key_flow_control_simulate;
    super.set_pose = vu_key_flow_control_set_pose;
    super.get_pose = vu_key_flow_control_get_pose;
    self->super = super;

    self->pose = VU_KEY_FLOW_CONTROL_INIT_POSE;
    self->speed_factor = VU_KEY_FLOW_CONTROL_INIT_SPEED_FACTOR;
    self->mouse_x = self->mouse_y = -1;

    return (VuControl *) self;
}

void vu_key_flow_control_key_event(VuControl *super, char key, bool pressed) {
    VuKeyFlowControl *self = (VuKeyFlowControl *) super;

    float front_speed = pressed ? self->speed_factor * VU_KEY_FLOW_CONTROL_FRONT_SPEED : 0;
    float right_speed = pressed ? self->speed_factor * VU_KEY_FLOW_CONTROL_RIGHT_SPEED : 0;
    float up_speed = pressed ? self->speed_factor * VU_KEY_FLOW_CONTROL_UP_SPEED : 0;
    float rotate_speed = pressed ? VU_KEY_FLOW_CONTROL_Z_ROTATE_SPEED : 0;

    if (key == 'w')
        self->speed_z = -front_speed;
    if (key == 'a')
        self->speed_x = -right_speed;
    if (key == 's')
        self->speed_z = front_speed;
    if (key == 'd')
        self->speed_x = right_speed;
    if (key == 'y')
        self->speed_y = up_speed;
    if (key == 'x')
        self->speed_y = -up_speed;
    if (key == 'q')
        self->speed_rot_z = -rotate_speed;
    if (key == 'e')
        self->speed_rot_z = rotate_speed;
}

void vu_key_flow_control_mouse_click_event(VuControl *super, float wnd_x, float wnd_y, int button, bool clicked) {
    VuKeyFlowControl *self = (VuKeyFlowControl *) super;

    if (clicked) {
        self->mouse_x = wnd_x;
        self->mouse_y = wnd_y;
    } else {
        self->mouse_x = -1;
        self->speed_rot_x = self->speed_rot_y = 0;
    }
}

void vu_key_flow_control_mouse_move_event(VuControl *super, float wnd_x, float wnd_y) {
    VuKeyFlowControl *self = (VuKeyFlowControl *) super;

    if (self->mouse_x >= 0) {
        float x_speed = (self->mouse_x - wnd_x) / VU_KEY_FLOW_CONTROL_MAX_SPEED_PIXELS;
        float y_speed = (self->mouse_y - wnd_y) / VU_KEY_FLOW_CONTROL_MAX_SPEED_PIXELS;

        // speed in range [-1 : 1]
        self->speed_rot_y = VU_KEY_FLOW_CONTROL_XY_ROTATE_SPEED * fmaxf(-1, fminf(1, x_speed));
        self->speed_rot_x = VU_KEY_FLOW_CONTROL_XY_ROTATE_SPEED * fmaxf(-1, fminf(1, y_speed));
    }
}

void vu_key_flow_control_mouse_scroll_event(VuControl *super, bool up) {
    VuKeyFlowControl *self = (VuKeyFlowControl *) super;

    if (up && self->speed_factor < 1) {
        self->speed_factor += VU_KEY_FLOW_CONTROL_SCROLL_SPEED_STEP;
        if (self->speed_factor > 1) {
            self->speed_factor = 1;
            log_info(PRELOG"maximum speed reached");
        }
    }

    if (!up && self->speed_factor > 0) {
        self->speed_factor -= VU_KEY_FLOW_CONTROL_SCROLL_SPEED_STEP;
        if (self->speed_factor <= 0) {
            self->speed_factor = 0;
            log_info(PRELOG"minimum speed reached = stop");
        }
    }
}

void vu_key_flow_control_simulate(VuControl *super, VuCamera *camera, double delta_time) {
    VuKeyFlowControl *self = (VuKeyFlowControl *) super;

    mat4 pose = self->pose;
    mat3 pose_rot = mat4_get_upper_left3(pose);

    if (self->speed_x != 0) {
        vec3 v_trans = pose.col[3].xyz;
        vec3 speed = {{self->speed_x * (float) delta_time, 0, 0}};
        vec3 add = mat3_mul_vec(pose_rot, speed);
        pose.col[3].xyz = vec3_add_vec(v_trans, add);
    }
    if (self->speed_y != 0) {
        vec3 v_trans = pose.col[3].xyz;
        vec3 speed = {{0, self->speed_y * (float) delta_time, 0}};
        vec3 add = mat3_mul_vec(pose_rot, speed);
        pose.col[3].xyz = vec3_add_vec(v_trans, add);
    }
    if (self->speed_z != 0) {
        vec3 v_trans = pose.col[3].xyz;
        vec3 speed = {{0, 0, self->speed_z * (float) delta_time}};
        vec3 add = mat3_mul_vec(pose_rot, speed);
        pose.col[3].xyz = vec3_add_vec(v_trans, add);
    }

    if (self->speed_rot_x != 0) {
        vec4 angle_axis;
        angle_axis.xyz = pose.col[0].xyz;
        angle_axis.w = self->speed_rot_x * (float) delta_time;
        mat3 rotation = mat3_rotation_from_angle_axis(angle_axis);
        pose_rot = mat3_mul_mat(rotation, pose_rot);
    }
    if (self->speed_rot_y != 0) {
        vec4 angle_axis;
        angle_axis.xyz = pose.col[1].xyz;
        angle_axis.w = self->speed_rot_y * (float) delta_time;
        mat3 rotation = mat3_rotation_from_angle_axis(angle_axis);
        pose_rot = mat3_mul_mat(rotation, pose_rot);
    }
    if (self->speed_rot_z != 0) {
        vec4 angle_axis;
        angle_axis.xyz = pose.col[2].xyz;
        angle_axis.w = self->speed_rot_z * (float) delta_time;
        mat3 rotation = mat3_rotation_from_angle_axis(angle_axis);
        pose_rot = mat3_mul_mat(rotation, pose_rot);
    }

    mat4_set_this_upper_left3(&pose, pose_rot);
    self->pose = pose;
    camera->pose = pose;
}

void vu_key_flow_control_set_pose(VuControl *super, mat4 pose) {
    VuKeyFlowControl *self = (VuKeyFlowControl *) super;
    self->pose = pose;
}

mat4 vu_key_flow_control_get_pose(const VuControl *super) {
    VuKeyFlowControl *self = (VuKeyFlowControl *) super;
    return self->pose;
}
