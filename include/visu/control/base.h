#ifndef VISU_CONTROL_BASE_H
#define VISU_CONTROL_BASE_H


#include <stdbool.h>

#include "visu/camera/base.h"

// for calling mouse_click_event...
static const int VU_CONTROL_MOUSE_BUTTON_LEFT = 1;
static const int VU_CONTROL_MOUSE_BUTTON_MIDDLE = 2;
static const int VU_CONTROL_MOUSE_BUTTON_RIGHT = 3;

struct VuControl;

typedef void (*vu_control_key_event_fun)(struct VuControl *self, char key, bool pressed);

typedef void (*vu_control_mouse_click_event_fun)(struct VuControl *self, float wnd_x, float wnd_y,
                                                 int button, bool clicked);

typedef void (*vu_control_mouse_move_event_fun)(struct VuControl *self, float wnd_x, float wnd_y);

typedef void (*vu_control_mouse_scroll_event_fun)(struct VuControl *self, bool up);

typedef void (*vu_control_simulate_fun)(struct VuControl *self, VuCamera *camera, double delta_time);

typedef void (*vu_control_set_pose_fun)(struct VuControl *self, mat4 pose);

typedef mat4 (*vu_control_get_pose_fun)(const struct VuControl *self);

/** Control super class */
typedef struct VuControl {
    vu_control_key_event_fun key_event;
    vu_control_mouse_click_event_fun mouse_click_event;
    vu_control_mouse_move_event_fun mouse_move_event;
    vu_control_mouse_scroll_event_fun mouse_scroll_event;
    vu_control_simulate_fun simulate;
    vu_control_set_pose_fun set_pose;
    vu_control_get_pose_fun get_pose;
} VuControl;

void vu_control_kill(VuControl **self_ptr);


#endif //VISU_CONTROL_BASE_H
