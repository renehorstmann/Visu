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

DEFAULT_TOP_LEFT_COLOR = np.array([0.1, 0.1, 0.6], dtype=np.float32)
DEFAULT_TOP_RIGHT_COLOR = np.array([0.1, 0.1, 0.6], dtype=np.float32)
DEFAULT_BOTTOM_LEFT_COLOR = np.array([0.1, 0.1, 0.4], dtype=np.float32)
DEFAULT_BOTTOM_RIGHT_COLOR = np.array([0.1, 0.1, 0.4], dtype=np.float32)

# /**
#  * Creates a background to render in non uniform 2D mode.
#  */
# VuRenderObject *vu_create_background(VuVisu *visu,
#                                      vec3 top_left_color, vec3 top_right_color,
#                                      vec3 bottom_left_color, vec3 bottom_right_color);
vulib.vu_create_background.argtypes = [visubase.VuVisu_p,
                                       mt.vec3, mt.vec3,
                                       mt.vec3, mt.vec3]
vulib.vu_create_background.restype = base.VuRenderObject_p


def create_background(vu: visubase.VisuBase,
                      top_left_color: mt.Array = DEFAULT_TOP_LEFT_COLOR,
                      top_right_color: mt.Array = DEFAULT_TOP_RIGHT_COLOR,
                      bottom_left_color: mt.Array = DEFAULT_BOTTOM_LEFT_COLOR,
                      bottom_right_color: mt.Array = DEFAULT_BOTTOM_RIGHT_COLOR) \
        -> base.RenderObject:
    top_left_color_cpy = np.float32(top_left_color)
    top_right_color_cpy = np.float32(top_right_color)
    bottom_left_color_cpy = np.float32(bottom_left_color)
    bottom_right_color_cpy = np.float32(bottom_right_color)
    ro = vulib.vu_create_background(vu._impl,
                                    mt.cast_into_vec3(top_left_color_cpy),
                                    mt.cast_into_vec3(top_right_color_cpy),
                                    mt.cast_into_vec3(bottom_left_color_cpy),
                                    mt.cast_into_vec3(bottom_right_color_cpy))
    return base.RenderObject(ro)
