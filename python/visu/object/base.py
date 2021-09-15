import numpy as np
import ctypes as ct
from typing import Tuple, List, Optional

from .. import types as t
from .. import mathctypes as mt
from .. import bindingbase as bb

from .. import plib
from .. import vulib

from .. import visubase


class VuLightOptions_s(ct.Structure):
    _fields_ = [
        ('light_source', mt.vec3),
        ('light_color', mt.vec3),
        ('light_power', bb.c_float),
        ('ambient_color', mt.vec3)
    ]


class VuRenderObject(ct.Structure):
    _fields_ = [
        ('impl', bb.c_void_p),
        ('pose', mt.mat4),
        ('used_pose', mt.mat4_p),
        ('update', bb.c_bool),
        ('kill_user_data', bb.c_bool),
        ('on_kill_callback', ct.CFUNCTYPE(None, bb.c_void_p, bb.c_void_p)),
        ('on_kill_callback_user_data', bb.c_void_p),
        ('visu_resource', visubase.VuVisuResource)
    ]


VuRenderObject_p = ct.POINTER(VuRenderObject)
VuRenderObject_p_p = ct.POINTER(VuRenderObject_p)

# /** @return: The data of a property, of NULL if not available. */
# void *vu_render_object_property(VuRenderObject *self, const char *name);
vulib.vu_render_object_property.argtypes = [VuRenderObject_p, bb.c_char_p]
vulib.vu_render_object_property.restype = bb.c_void_p

# /** @return: the type (string) of a property (or NULL if not available). */
# const char *vu_render_object_property_type(VuRenderObject *self, const char *name);
vulib.vu_render_object_property_type.argtypes = [VuRenderObject_p, bb.c_char_p]
vulib.vu_render_object_property_type.restype = bb.c_char_p

# /** @return: A list (on heap) containing all available property names. (Don't forget to call free(list); ) */
# const char **vu_render_object_property_names_as_list_on_heap(VuRenderObject *self);
vulib.vu_render_object_property_names_as_list_on_heap.argtypes = [VuRenderObject_p]
vulib.vu_render_object_property_names_as_list_on_heap.restype = ct.POINTER(bb.c_char_p)

# p_rhc_free
plib.p_rhc_free.argtypes = [bb.c_void_p]

# /**
#  * Kills a render object (says Visu to delete it...).
#  * This call is not necessary if Visu will be killed also, (Visu will kill all render objects automatically)
#  */
# void vu_render_object_kill(VuRenderObject **self_ptr);
vulib.vu_render_object_kill.argtypes = [VuRenderObject_p_p]

# /**
#  * Uses OpenGL-calls to render the object.
#  */
# void vu_render_object_render(VuRenderObject *self);
vulib.vu_render_object_render.argtypes = [VuRenderObject_p]


class PropertyNameList:
    def __init__(self, property_names_on_heap):
        self._impl = property_names_on_heap
        self.list = []
        it = self._impl
        while it is not None and it.contents is not None and it.contents.value is not None:
            self.list.append(it.contents.value.decode())

            # increment pointer by 1
            memaddr = ct.cast(it, bb.c_void_p).value
            memaddr += ct.sizeof(it._type_)
            it = ct.cast(bb.c_void_p(memaddr), ct.POINTER(bb.c_char_p))

    def __del__(self):
        plib.p_rhc_free(self._impl)
        self._impl = None


class RenderObject:
    def __init__(self, render_object: VuRenderObject_p):
        self._impl = render_object
        self._used_pose_view_ = None

    def property(self, name: str):
        return vulib.vu_render_object_property(self._impl, name.encode())

    def property_type(self, name: str):
        return vulib.vu_render_object_property_type(self._impl, name.encode())

    def property_names_as_list(self):
        return PropertyNameList(vulib.vu_render_object_property_names_as_list_on_heap(self._impl))

    def kill(self):
        vulib.vu_render_object_kill(bb.ref(self._impl))
        self._impl = None

    def render(self):
        vulib.vu_render_object_render(self._impl)

    def update(self):
        self._impl.contents.update = True

    def pose(self):
        return mt.cast_from_mat4(self._impl.contents.pose)

    def set_used_pose(self, mat: np.ndarray):
        self._used_pose_view_ = mat  # to prevent deallocation
        self._impl.contents.used_pose = mt.cast_into_mat4_p(self._used_pose_view_)

    def get_used_pose(self):
        return self._used_pose_view_
