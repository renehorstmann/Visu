import numpy as np
import ctypes as ct
from typing import Tuple, List, Optional

from . import types as t
from . import mathctypes as mt
from . import bindingbase as bb

from . import plib
from . import vulib

from . import control

vu_visu_resource_unload_fun = ct.CFUNCTYPE(None, bb.c_void_p)
vu_visu_resource_kill_fun = ct.CFUNCTYPE(None, bb.c_void_p)


class VuVisuResource(ct.Structure):
    _fields_ = [
        ('impl', bb.c_void_p),
        ('unload', vu_visu_resource_unload_fun),
        ('kill', vu_visu_resource_kill_fun)
    ]


vu_visu_render_fun = ct.CFUNCTYPE(None, bb.c_void_p, bb.c_void_p)


class VuVisu(ct.Structure):
    _fields_ = [
        ('impl', bb.c_void_p),
        ('multisample', bb.c_int),
        ('camera', bb.c_void_p),
        ('opt_control', control.VuControl_p),
        ('render_function', vu_visu_render_fun),
        ('render_function_user_data', bb.c_void_p),
        ('delta_time', bb.c_double),
        ('screen_width', bb.c_int),
        ('screen_height', bb.c_int),
        ('view', mt.mat4),
        ('projection', mt.mat4)
    ]


VuVisu_p = ct.POINTER(VuVisu)
VuVisu_p_p = ct.POINTER(VuVisu_p)

# VuVisu *vu_visu_new(bool orthogonal,
#                    void (*render_function)(VuVisu *visu, void *user_data),
#                    void *render_function_user_data);
vulib.vu_visu_new.argtypes = [bb.c_bool,
                              vu_visu_render_fun,
                              bb.c_void_p]
vulib.vu_visu_new.restype = VuVisu_p

# void vu_visu_kill(VuVisu **self_ptr);
vulib.vu_visu_kill.argtypes = [VuVisu_p_p]

# /** Should be called with active OpenGL context to init Visu */
# void vu_visu_realize(VuVisu *self);
vulib.vu_visu_realize.argtypes = [VuVisu_p]

# /** Should be called when loosing the OpenGL context to clear Visu */
# void vu_visu_unrealize(VuVisu *self);
vulib.vu_visu_unrealize.argtypes = [VuVisu_p]

# /**
#  * Should be called with active OpenGL context, to render.
#  * This method sets up OpenGL for the call of the user render function (render_function).
#  */
# void vu_visu_render(VuVisu *self, int width, int height);
vulib.vu_visu_render.argtypes = [VuVisu_p, bb.c_int, bb.c_int]

# /**
#  * Sets up 3D mode.
#  * (Depth test + camera view, projection)
#  */
# void vu_visu_3D(VuVisu *self);
vulib.vu_visu_3D.argtypes = [VuVisu_p]

# /**
#  * Sets up 2D mode.
#  * (No Depth test + eye view, 2d projection)
#  * @param uniform_view: If false: projection mat will be eye.
#  * if true: projection mat will be set to guarantee a uniform 1 quad.
#  */
# void vu_visu_2D(VuVisu *self, bool uniform_view);
vulib.vu_visu_2D.argtypes = [VuVisu_p, bb.c_bool]


class VisuBase:
    def __init__(self, visu_impl: VuVisu_p):
        self._impl = visu_impl

    def realize(self):
        vulib.vu_visu_realize(self._impl)

    def unrealize(self):
        vulib.vu_visu_unrealize(self._impl)

    def render(self, width: int, height: int):
        vulib.vu_visu_render(self._impl, width, height)

    def mode3D(self):
        vulib.vu_visu_3D(self._impl)

    def mode2D(self, uniform_view: bool):
        vulib.vu_visu_2D(self._impl, uniform_view)

    def get_control(self) -> Optional[control.Control]:
        if self._impl.contents.opt_control is None:
            return None
        return control.Control(self._impl.contents.opt_control)


class Visu(VisuBase):
    def __init__(self, orthogonal: bool, render_function, render_function_user_data):
        self.render_function = render_function
        self.render_function_user_data = render_function_user_data

        def vu_render_function(visu_ptr: bb.c_void_p, data: bb.c_void_p):
            self.render_function(self, self.render_function_user_data)

        self._vu_render_function = vu_visu_render_fun(vu_render_function)

        visu_impl = vulib.vu_visu_new(orthogonal, self._vu_render_function, None)
        super().__init__(visu_impl)

    def __del__(self):
        vulib.vu_visu_kill(bb.ref(self._impl))
        self._impl = None
