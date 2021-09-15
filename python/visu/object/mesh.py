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

MODE_SURFACES = 0
MODE_WIREFRAME = 1
MODE_SURFACES_AND_WIREFRAME = 2
MODE_NUM_MODES = 3

vu_meshmode = bb.c_int


class VuMesh(ct.Structure):
    _fields_ = [
        ('super', base.VuRenderObject),

        ('points', t.pCloud),
        ('indices', t.pMeshIndices),
        ('color', mt.vec4),
        ('wire_color', mt.vec4),
        ('wire_size', bb.c_float),
        ('mesh_mode', vu_meshmode),
        ('light_options', base.VuLightOptions_s)
    ]


VuMesh_p = ct.POINTER(VuMesh)

# /**
#  * Creates a new render object (new + init).
#  * The return object is the super class for all render objects, it can be cast to the desired form.
#  * Render it in visus render_function with vu_render_object_render.
#  *
#  * wire_color = surface_color
#  * wire_size = 1
#  * mesh_mode = VU_MESH_MODE_SURFACES
#  */
# VuRenderObject *vu_mesh_new(VuVisu *visu, pCloud points, pMeshIndices indices, vec4 color);
vulib.vu_mesh_new.argtypes = [visubase.VuVisu_p, t.pCloud, t.pMeshIndices, mt.vec4]
vulib.vu_mesh_new.restype = base.VuRenderObject_p


class Mesh(base.RenderObject):
    def __init__(self, vu: visubase.VisuBase,
                 points: np.ndarray,
                 indices: np.ndarray,
                 color: mt.Array):
        self.points = points.copy()  # prevent deallocation
        self.indices = indices.copy()
        ro = vulib.vu_mesh_new(vu._impl,
                               t.cast_into_pCloud(self.points),
                               t.cast_into_pMeshIndices(self.indices),
                               mt.cast_array_vec4(color))
        super().__init__(ro)

    def update(self):
        super().update()
        ro = ct.cast(self._impl, VuMesh_p)
        ro.contents.points = t.cast_into_pCloud(self.points)
        ro.contents.indices = t.cast_into_pMeshIndices(self.indices)


class VuMeshColored(ct.Structure):
    _fields_ = [
        ('super', base.VuRenderObject),

        ('points', t.pCloud),
        ('colors', t.pCloud),
        ('indices', t.pMeshIndices),
        ('wire_size', bb.c_float),
        ('mesh_mode', vu_meshmode),
        ('light_options', base.VuLightOptions_s)
    ]


VuMeshColored_p = ct.POINTER(VuMeshColored)

# /**
#  * Creates a new render object (new + init).
#  * The return object is the super class for all render objects, it can be cast to the desired form.
#  * Render it in visus render_function with vu_render_object_render.
#  *
#  * wire_size = 1
#  * mesh_mode = VU_MESH_MODE_SURFACES
#  */
# VuRenderObject *vu_mesh_colored_new(VuVisu *visu, pCloud points, pCloud colors, pMeshIndices indices);
vulib.vu_mesh_colored_new.argtypes = [visubase.VuVisu_p, t.pCloud, t.pCloud, t.pMeshIndices]
vulib.vu_mesh_colored_new.restype = base.VuRenderObject_p


class MeshColored(base.RenderObject):
    def __init__(self, vu: visubase.VisuBase,
                 points: np.ndarray,
                 colors: np.ndarray,
                 indices: np.ndarray):
        self.points = points.copy()  # prevent deallocation
        self.colors = colors.copy()
        self.indices = indices.copy()
        ro = vulib.vu_mesh_colored_new(vu._impl,
                                       t.cast_into_pCloud(self.points),
                                       t.cast_into_pCloud(self.colors),
                                       t.cast_into_pMeshIndices(self.indices))
        super().__init__(ro)

    def update(self):
        super().update()
        ro = ct.cast(self._impl, VuMeshColored_p)
        ro.contents.points = t.cast_into_pCloud(self.points)
        ro.contents.colors = t.cast_into_pCloud(self.colors)
        ro.contents.indices = t.cast_into_pMeshIndices(self.indices)


class VuMeshColoredMatt(ct.Structure):
    _fields_ = [
        ('super', base.VuRenderObject),

        ('points', t.pCloud),
        ('colors', t.pCloud),
        ('indices', t.pMeshIndices),
        ('wire_size', bb.c_float),
        ('mesh_mode', vu_meshmode)
    ]


VuMeshColoredMatt_p = ct.POINTER(VuMeshColoredMatt)

# /**
#  * Creates a new render object (new + init).
#  * The return object is the super class for all render objects, it can be cast to the desired form.
#  * Render it in visus render_function with vu_render_object_render.
#  *
#  * wire_size = 1
#  * mesh_mode = VU_MESH_MODE_SURFACES
#  */
# VuRenderObject *vu_mesh_colored_matt_new(VuVisu *visu, pCloud points, pCloud colors, pMeshIndices indices);
vulib.vu_mesh_colored_matt_new.argtypes = [visubase.VuVisu_p, t.pCloud, t.pCloud, t.pMeshIndices]
vulib.vu_mesh_colored_matt_new.restype = base.VuRenderObject_p


class MeshColoredMatt(base.RenderObject):
    def __init__(self, vu: visubase.VisuBase,
                 points: np.ndarray,
                 colors: np.ndarray,
                 indices: np.ndarray):
        self.points = points.copy()  # prevent deallocation
        self.colors = colors.copy()
        self.indices = indices.copy()
        ro = vulib.vu_mesh_colored_matt_new(vu._impl,
                                            t.cast_into_pCloud(self.points),
                                            t.cast_into_pCloud(self.colors),
                                            t.cast_into_pMeshIndices(self.indices))
        super().__init__(ro)

    def update(self):
        super().update()
        ro = ct.cast(self._impl, VuMeshColoredMatt_p)
        ro.contents.points = t.cast_into_pCloud(self.points)
        ro.contents.colors = t.cast_into_pCloud(self.colors)
        ro.contents.indices = t.cast_into_pMeshIndices(self.indices)


class VuRawMesh(ct.Structure):
    _fields_ = [
        ('super', base.VuRenderObject),

        ('points', t.pCloud),
        ('normals', t.pCloud),
        ('color', mt.vec4),
        ('wire_color', mt.vec4),
        ('wire_size', bb.c_float),
        ('mesh_mode', vu_meshmode),
        ('light_options', base.VuLightOptions_s)
    ]


VuRawMesh_p = ct.POINTER(VuRawMesh)

# /**
#  * Creates a new render object (new + init).
#  * The return object is the super class for all render objects, it can be cast to the desired form.
#  * Render it in visus render_function with vu_render_object_render.
#  *
#  * wire_color = surface_color
#  * wire_size = 1
#  * mesh_mode = VU_MESH_MODE_SURFACES
#  */
# VuRenderObject *vu_raw_mesh_new(VuVisu *visu, pCloud points, pCloud normals, vec4 color);
vulib.vu_raw_mesh_new.argtypes = [visubase.VuVisu_p, t.pCloud, t.pCloud, mt.vec4]
vulib.vu_raw_mesh_new.restype = base.VuRenderObject_p


class RawMesh(base.RenderObject):
    def __init__(self, vu: visubase.VisuBase,
                 points: np.ndarray,
                 normals: np.ndarray,
                 color: mt.Array):
        self.points = points  # prevent deallocation
        self.normals = normals
        ro = vulib.vu_raw_mesh_new(vu._impl,
                                   t.cast_into_pCloud(self.points),
                                   t.cast_into_pCloud(self.normals),
                                   mt.cast_array_vec4(color))
        super().__init__(ro)

    def update(self):
        super().update()
        ro = ct.cast(self._impl, VuRawMesh_p)
        ro.contents.points = t.cast_into_pCloud(self.points)
        ro.contents.normals = t.cast_into_pCloud(self.normals)


class VuRawMeshColored(ct.Structure):
    _fields_ = [
        ('super', base.VuRenderObject),

        ('points', t.pCloud),
        ('normals', t.pCloud),
        ('colors', t.pCloud),
        ('wire_size', bb.c_float),
        ('mesh_mode', vu_meshmode),
        ('light_options', base.VuLightOptions_s)
    ]


VuRawMeshColored_p = ct.POINTER(VuRawMeshColored)

# /**
#  * Creates a new render object (new + init).
#  * The return object is the super class for all render objects, it can be cast to the desired form.
#  * Render it in visus render_function with vu_render_object_render.
#  *
#  * wire_size = 1
#  * mesh_mode = VU_MESH_MODE_SURFACES
#  */
# VuRenderObject *vu_raw_mesh_colored_new(VuVisu *visu, pCloud points, pCloud normals, pCloud colors);
vulib.vu_raw_mesh_colored_new.argtypes = [visubase.VuVisu_p, t.pCloud, t.pCloud, t.pCloud]
vulib.vu_raw_mesh_colored_new.restype = base.VuRenderObject_p


class RawMeshColored(base.RenderObject):
    def __init__(self, vu: visubase.VisuBase,
                 points: np.ndarray,
                 normals: np.ndarray,
                 colors: np.ndarray):
        self.points = points  # prevent deallocation
        self.normals = normals
        self.colors = colors
        ro = vulib.vu_raw_mesh_colored_new(vu._impl,
                                           t.cast_into_pCloud(self.points),
                                           t.cast_into_pCloud(self.normals),
                                           t.cast_into_pCloud(self.colors))
        super().__init__(ro)

    def update(self):
        super().update()
        ro = ct.cast(self._impl, VuRawMeshColored_p)
        ro.contents.points = t.cast_into_pCloud(self.points)
        ro.contents.normals = t.cast_into_pCloud(self.normals)
        ro.contents.colors = t.cast_into_pCloud(self.colors)
