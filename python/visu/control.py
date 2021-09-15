import numpy as np
import ctypes as ct
from typing import Tuple, List, Optional

from . import types as t
from . import mathctypes as mt
from . import bindingbase as bb

from . import plib
from . import vulib

MOUSE_BUTTON_LEFT = 1
MOUSE_BUTTON_MIDDLE = 2
MOUSE_BUTTON_RIGHT = 3

# forward cast dont works with ctypes, so we cast the function ptr in the functions below
_fun_p = ct.CFUNCTYPE(None)


class VuControl(ct.Structure):
    _fields_ = [
        ('key_event', _fun_p),
        ('mouse_click_event', _fun_p),
        ('mouse_move_event', _fun_p),
        ('mouse_scroll_event', _fun_p),
        ('simulate', _fun_p),
        ('set_pose', _fun_p),
        ('get_pose', _fun_p)
    ]


VuControl_p = ct.POINTER(VuControl)
VuControl_p_p = ct.POINTER(VuControl_p)


# C interface:
#
# /** Control super class */
# typedef struct VuControl {
#     vu_control_key_event_fun key_event;
#     vu_control_mouse_click_event_fun mouse_click_event;
#     vu_control_mouse_move_event_fun mouse_move_event;
#     vu_control_mouse_scroll_event_fun mouse_scroll_event;
#     vu_control_simulate_fun simulate;
#     vu_control_set_pose_fun set_pose;
#     vu_control_get_pose_fun get_pose;
# } VuControl;

class Control:
    def __init__(self, control: VuControl_p):
        self._impl = control

    def key_event(self, key: int, pressed: bool):
        fun = ct.cast(self._impl.contents.key_event,
                      ct.CFUNCTYPE(None, VuControl_p, ct.c_char, ct.c_bool))
        fun(self._impl, key, pressed)

    def mouse_click_event(self, wnd_x: float, wnd_y: float, button: int, clicked: bool):
        fun = ct.cast(self._impl.contents.mouse_click_event,
                      ct.CFUNCTYPE(None, VuControl_p,
                                   ct.c_float, ct.c_float, ct.c_int, ct.c_bool))
        fun(self._impl, wnd_x, wnd_y, button, clicked)

    def mouse_move_event(self, wnd_x: float, wnd_y: float):
        fun = ct.cast(self._impl.contents.mouse_move_event,
                      ct.CFUNCTYPE(None, VuControl_p, ct.c_float, ct.c_float))
        fun(self._impl, wnd_x, wnd_y)

    def mouse_scroll_event(self, up: bool):
        fun = ct.cast(self._impl.contents.mouse_scroll_event,
                      ct.CFUNCTYPE(None, VuControl_p, ct.c_bool))
        fun(self._impl, up)

    def set_pose(self, pose: np.ndarray):
        fun = ct.cast(self._impl.contents.set_pose,
                      ct.CFUNCTYPE(None, VuControl_p, mt.vec4))
        fun(self._impl, mt.cast_into_mat4(pose))

    def get_pose(self) -> np.ndarray:
        fun = ct.cast(self._impl.contents.get_pose,
                      ct.CFUNCTYPE(mt.mat4, VuControl_p))
        pose = fun(self._impl)
        return mt.cast_from_mat4(pose)
