import numpy as np
import ctypes as ct
from typing import Tuple, List, Optional

from .. import types as t
from .. import mathctypes as mt
from .. import bindingbase as bb

from .. import plib
from .. import vulib

from .. import visubase
from . import base

MODE_LINES = 0
MODE_LINE_STRIP = 1
MODE_LINE_LOOP = 2
MODE_NUM_MODES = 3

vu_linemode = bb.c_int


class VuLines(ct.Structure):
    _fields_ = [
        ('super', base.VuRenderObject),

        ('points', t.pCloud),
        ('color', mt.vec4),
        ('line_size', bb.c_float),
        ('line_mode', vu_linemode)
    ]


VuLines_p = ct.POINTER(VuLines)

# /**
#  * Creates a new render object (new + init).
#  * The return object is the super class for all render objects, it can be cast to the desired form.
#  * Render it in visus render_function with vu_render_object_render.
#  */
# VuRenderObject *vu_lines_new(VuVisu *visu, pCloud points, vec4 color, float line_size,
#                              enum vu_linemode line_mode);
vulib.vu_lines_new.argtypes = [visubase.VuVisu_p, t.pCloud, mt.vec4, bb.c_float,
                               vu_linemode]
vulib.vu_lines_new.restype = base.VuRenderObject_p


class Lines(base.RenderObject):
    def __init__(self, vu: visubase.VisuBase,
                 points: np.ndarray,
                 color: mt.Array,
                 point_size: float,
                 line_mode: int):
        self.points = points.copy()  # prevent deallocation
        ro = vulib.vu_lines_new(vu._impl,
                                t.cast_into_pCloud(self.points),
                                mt.cast_array_vec4(color),
                                point_size,
                                line_mode)
        super().__init__(ro)

    def update(self):
        super().update()
        ro = ct.cast(self._impl, VuLines_p)
        ro.contents.points = t.cast_into_pCloud(self.points)


class VuLinesColored(ct.Structure):
    _fields_ = [
        ('super', base.VuRenderObject),

        ('points', t.pCloud),
        ('colors', t.pCloud),
        ('line_size', bb.c_float),
        ('line_mode', bb.c_int)
    ]


VuLinesColored_p = ct.POINTER(VuLinesColored)

# /**
#  * Creates a new render object (new + init).
#  * The return object is the super class for all render objects, it can be cast to the desired form.
#  * Render it in visus render_function with vu_render_object_render.
#  */
# VuRenderObject *vu_lines_colored_new(VuVisu *visu, pCloud points, pCloud colors, float line_size,
#                                      enum vu_linemode line_mode);
vulib.vu_lines_colored_new.argtypes = [visubase.VuVisu_p, t.pCloud, t.pCloud, bb.c_float,
                                       vu_linemode]
vulib.vu_lines_colored_new.restype = base.VuRenderObject_p


class LinesColored(base.RenderObject):
    def __init__(self, vu: visubase.VisuBase,
                 points: np.ndarray,
                 colors: np.ndarray,
                 point_size: float,
                 line_mode: int):
        self.points = points.copy()  # prevent deallocation
        self.colors = colors.copy()
        ro = vulib.vu_lines_colored_new(vu._impl,
                                        t.cast_into_pCloud(self.points),
                                        t.cast_into_pCloud(self.colors),
                                        point_size,
                                        line_mode)
        super().__init__(ro)

    def update(self):
        super().update()
        ro = ct.cast(self._impl, VuLinesColored_p)
        ro.contents.points = t.cast_into_pCloud(self.points)
        ro.contents.colors = t.cast_into_pCloud(self.colors)
