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


class VuPoints(ct.Structure):
    _fields_ = [
        ('super', base.VuRenderObject),

        ('points', t.pCloud),
        ('color', mt.vec4),
        ('point_size', bb.c_float)
    ]


VuPoints_p = ct.POINTER(VuPoints)

# /**
#  * Creates a new render object (new + init).
#  * The return object is the super class for all render objects, it can be cast to the desired form.
#  * Render it in visus render_function with vu_render_object_render.
#  */
# VuRenderObject *vu_points_new(VuVisu *visu, pCloud points, vec4 color, float point_size);
vulib.vu_points_new.argtypes = [visubase.VuVisu_p, t.pCloud, mt.vec4, bb.c_float]
vulib.vu_points_new.restype = base.VuRenderObject_p


class Points(base.RenderObject):
    def __init__(self, vu: visubase.VisuBase,
                 points: np.ndarray,
                 color: mt.Array,
                 point_size: float):
        self.points = points.copy()  # prevent deallocation
        ro = vulib.vu_points_new(vu._impl,
                                 t.cast_into_pCloud(self.points),
                                 mt.cast_array_vec4(color),
                                 point_size)
        super().__init__(ro)

    def update(self):
        super().update()
        ro = ct.cast(self._impl, VuPoints_p)
        ro.contents.points = t.cast_into_pCloud(self.points)


class VuPointsColored(ct.Structure):
    _fields_ = [
        ('super', base.VuRenderObject),

        ('points', t.pCloud),
        ('colors', t.pCloud),
        ('point_size', bb.c_float)
    ]


VuPointsColored_p = ct.POINTER(VuPointsColored)

# /**
#  * Creates a new render object (new + init).
#  * The return object is the super class for all render objects, it can be cast to the desired form.
#  * Render it in visus render_function with vu_render_object_render.
#  */
# VuRenderObject *vu_points_colored_new(VuVisu *visu, pCloud points, pCloud colors, float point_size);
vulib.vu_points_colored_new.argtypes = [visubase.VuVisu_p, t.pCloud, t.pCloud, bb.c_float]
vulib.vu_points_colored_new.restype = base.VuRenderObject_p


class PointsColored(base.RenderObject):
    def __init__(self, vu: visubase.VisuBase,
                 points: np.ndarray,
                 colors: np.ndarray,
                 point_size: float):
        self.points = points.copy()  # prevent deallocation
        self.colors = colors.copy()
        ro = vulib.vu_points_colored_new(vu._impl,
                                         t.cast_into_pCloud(self.points),
                                         t.cast_into_pCloud(self.colors),
                                         point_size)
        super().__init__(ro)

    def update(self):
        super().update()
        ro = ct.cast(self._impl, VuPointsColored_p)
        ro.contents.points = t.cast_into_pCloud(self.points)
        ro.contents.colors = t.cast_into_pCloud(self.colors)


class VuPointNormals(ct.Structure):
    _fields_ = [
        ('super', base.VuRenderObject),

        ('points', t.pCloud),
        ('normals', t.pCloud),
        ('color', mt.vec4),
        ('point_size', bb.c_float),
        ('light_options', base.VuLightOptions_s)
    ]


VuPointNormals_p = ct.POINTER(VuPointNormals)

# /**
#  * Creates a new render object (new + init).
#  * The return object is the super class for all render objects, it can be cast to the desired form.
#  * Render it in visus render_function with vu_render_object_render.
#  */
# VuRenderObject *vu_point_normals_new(VuVisu *visu, pCloud points, pCloud normals,
#                                      vec4 color, float point_size);
vulib.vu_point_normals_new.argtypes = [visubase.VuVisu_p, t.pCloud, t.pCloud,
                                       mt.vec4, bb.c_float]
vulib.vu_point_normals_new.restype = base.VuRenderObject_p


class PointNormals(base.RenderObject):
    def __init__(self, vu: visubase.VisuBase,
                 points: np.ndarray,
                 normals: np.ndarray,
                 color: mt.Array,
                 point_size: float):
        self.points = points.copy()  # prevent deallocation
        self.normals = normals.copy()
        ro = vulib.vu_point_normals_new(vu._impl,
                                        t.cast_into_pCloud(self.points),
                                        t.cast_into_pCloud(self.normals),
                                        mt.cast_array_vec4(color),
                                        point_size)
        super().__init__(ro)

    def update(self):
        super().update()
        ro = ct.cast(self._impl, VuPointNormals_p)
        ro.contents.points = t.cast_into_pCloud(self.points)
        ro.contents.normals = t.cast_into_pCloud(self.normals)


class VuPointNormalsColored(ct.Structure):
    _fields_ = [
        ('super', base.VuRenderObject),

        ('points', t.pCloud),
        ('normals', t.pCloud),
        ('colors', t.pCloud),
        ('point_size', bb.c_float),
        ('light_options', base.VuLightOptions_s)
    ]


VuPointNormalsColored_p = ct.POINTER(VuPointNormalsColored)

# /**
#  * Creates a new render object (new + init).
#  * The return object is the super class for all render objects, it can be cast to the desired form.
#  * Render it in visus render_function with vu_render_object_render.
#  */
# VuRenderObject *vu_point_normals_colored_new(VuVisu *visu, pCloud points, pCloud normals,
#                                              pCloud colors, float point_size);
vulib.vu_point_normals_colored_new.argtypes = [visubase.VuVisu_p, t.pCloud, t.pCloud,
                                               t.pCloud, bb.c_float]
vulib.vu_point_normals_colored_new.restype = base.VuRenderObject_p


class PointNormalsColored(base.RenderObject):
    def __init__(self, vu: visubase.VisuBase,
                 points: np.ndarray,
                 normals: np.ndarray,
                 colors: np.ndarray,
                 point_size: float):
        self.points = points.copy()  # prevent deallocation
        self.normals = normals.copy()
        self.colors = colors.copy()
        ro = vulib.vu_point_normals_colored_new(vu._impl,
                                                t.cast_into_pCloud(self.points),
                                                t.cast_into_pCloud(self.normals),
                                                t.cast_into_pCloud(self.colors),
                                                point_size)
        super().__init__(ro)

    def update(self):
        super().update()
        ro = ct.cast(self._impl, VuPointNormalsColored_p)
        ro.contents.points = t.cast_into_pCloud(self.points)
        ro.contents.normals = t.cast_into_pCloud(self.normals)
        ro.contents.colors = t.cast_into_pCloud(self.colors)
