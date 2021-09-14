#ifndef VISU_CONTROL_KEYFLOW_H
#define VISU_CONTROL_KEYFLOW_H


#include "base.h"

static const float VU_KEY_FLOW_CONTROL_FRONT_SPEED = 2000;
static const float VU_KEY_FLOW_CONTROL_RIGHT_SPEED = 1000;
static const float VU_KEY_FLOW_CONTROL_UP_SPEED = 1000;
static const float VU_KEY_FLOW_CONTROL_Z_ROTATE_SPEED = 1;
static const float VU_KEY_FLOW_CONTROL_XY_ROTATE_SPEED = 3;
static const float VU_KEY_FLOW_CONTROL_MAX_SPEED_PIXELS = 1000;
static const float VU_KEY_FLOW_CONTROL_SCROLL_SPEED_STEP = 1.0f / 50.0f;


static const mat4 VU_KEY_FLOW_CONTROL_INIT_POSE = {{
                                                           0, 1, 0, 0,
                                                           -0.7071067811865475f, 0, 0.7071067811865475f, 0,
                                                           0.7071067811865475f, 0, 0.7071067811865475f, 0,
                                                           1000, 0, 1000, 1
                                                   }};
static const float VU_KEY_FLOW_CONTROL_INIT_SPEED_FACTOR = 0.25f;

/**
 * Controls the PerspectiveCamera via qweasdyx keys and the mouse.
 */
typedef struct {
    VuControl super;

    mat4 pose;
    float speed_factor;
    float speed_x, speed_y, speed_z;
    float speed_rot_x, speed_rot_y, speed_rot_z;
    float mouse_x, mouse_y;
} VuKeyFlowControl;

VuControl *vu_key_flow_control_new();

void vu_key_flow_control_key_event(VuControl *super, char key, bool pressed);

void vu_key_flow_control_mouse_click_event(VuControl *super, float wnd_x, float wnd_y, int button, bool clicked);

void vu_key_flow_control_mouse_move_event(VuControl *super, float wnd_x, float wnd_y);

void vu_key_flow_control_mouse_scroll_event(VuControl *super, bool up);

void vu_key_flow_control_simulate(VuControl *super, VuCamera *camera, double delta_time);

void vu_key_flow_control_set_pose(VuControl *super, mat4 pose);

mat4 vu_key_flow_control_get_pose(const VuControl *super);


#endif //VISU_CONTROL_KEYFLOW_H
