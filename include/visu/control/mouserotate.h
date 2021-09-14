#ifndef VISU_CONTROL_MOUSEROTATE_H
#define VISU_CONTROL_MOUSEROTATE_H


#include "base.h"

static const float VU_MOUSE_ROTATE_CONTROL_XY_ROTATE_SPEED = 5;
static const float VU_MOUSE_ROTATE_CONTROL_XY_MOVE_SPEED = 2000;
static const float VU_MOUSE_ROTATE_CONTROL_MAX_SPEED_PIXELS = 1000;
static const float VU_MOUSE_ROTATE_CONTROL_SCROLL_FACTOR = 1.2f;

static const mat4 VU_MOUSE_ROTATE_CONTROL_INIT_CENTER_POSE = {{
                                                                      0, 1, 0, 0,
                                                                      -0.7071067811865475f, 0, 0.7071067811865475f, 0,
                                                                      0.7071067811865475f, 0, 0.7071067811865475f, 0,
                                                                      0, 0, 0, 1
                                                              }};

static const float VU_MOUSE_ROTATE_CONTROL_INIT_CAMERA_DISTANCE = 20000;


/**
 * Controls the OrthogonalCamera with the mouse
 */
typedef struct {
    VuControl super;

    mat4 center_pose;
    float camera_distance;
    float speed_x, speed_y;
    float speed_rot_x, speed_rot_y;
    float scroll;
    float mouse_move_pos_x, mouse_move_pos_y;
    float mouse_rot_pos_x, mouse_rot_pos_y;
} VuMouseRotateControl;

VuControl *vu_mouse_rotate_control_new();

void vu_mouse_rotate_control_key_event(VuControl *super, char key, bool pressed);

void vu_mouse_rotate_control_mouse_click_event(VuControl *super,
                                               float wnd_x, float wnd_y, int button, bool clicked);

void vu_mouse_rotate_control_mouse_move_event(VuControl *super, float wnd_x, float wnd_y);

void vu_mouse_rotate_control_mouse_scroll_event(VuControl *super, bool up);

void vu_mouse_rotate_control_simulate(VuControl *super, VuCamera *camera, double delta_time);

void vu_mouse_rotate_control_set_pose(VuControl *super, mat4 pose);

mat4 vu_mouse_rotate_control_get_pose(const VuControl *super);


#endif //VISU_CONTROL_MOUSEROTATE_H
