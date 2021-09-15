import numpy as np
import ctypes as ct
from typing import Tuple, List, Optional

from . import types as t
from . import mathctypes as mt
from . import bindingbase as bb

from . import plib
from . import vulib

from . import visubase
from . import object

MAX_QUIT_AT_KEYS = 8
MAX_TRACKBARS = 8
MAX_RENDER_OBJECTS = 1024

WHITE = np.array([1, 1, 1, 1], dtype=np.float32)
BLACK = np.array([0, 0, 0, 1], dtype=np.float32)
RED = np.array([1, 0, 0, 1], dtype=np.float32)
GREEN = np.array([0, 1, 0, 1], dtype=np.float32)
BLUE = np.array([0, 0, 1, 1], dtype=np.float32)
YELLOW = np.array([1, 1, 0, 1], dtype=np.float32)
CYAN = np.array([0, 1, 1, 1], dtype=np.float32)
MAGENTA = np.array([1, 0, 1, 1], dtype=np.float32)

# forward cast dont works with ctypes
_fun_p = ct.CFUNCTYPE(None)


# added Vu namespace to distinguish between C and Python
class VuSVisuTrackbar(ct.Structure):
    _fields_ = [
        ('name', bb.c_char_p),
        ('min', bb.c_double),
        ('max', bb.c_double),
        ('step', bb.c_double),
        ('init_value', bb.c_double),
        ('callback', _fun_p),
        ('callback_user_data', bb.c_void_p)
    ]


svisu_mouse_ray_callback_fun = ct.CFUNCTYPE(None, bb.c_void_p, mt.vec3, mt.vec3, bb.c_void_p)
svisu_render_callback_fun = ct.CFUNCTYPE(None, bb.c_void_p, bb.c_void_p)


# added Vu namespace to distinguish between C and Python
class VuSVisu(ct.Structure):
    _fields_ = [
        ('wnd_size_x', bb.c_int),
        ('wnd_size_y', bb.c_int),
        ('wnd_move_x', bb.c_int),
        ('wnd_move_y', bb.c_int),
        ('quit_at_keys', bb.c_char * MAX_QUIT_AT_KEYS),
        ('mouse_ray_callback', svisu_mouse_ray_callback_fun),
        ('mouse_ray_callback_user_data', bb.c_void_p),
        ('render_callback', svisu_render_callback_fun),
        ('render_callback_user_data', bb.c_void_p),
        ('trackbars', VuSVisuTrackbar * MAX_TRACKBARS),
        ('trackbars_size', bb.c_int),
        ('visu', visubase.VuVisu_p),
        ('_last_pressed_key', bb.c_char),
        ('_init_as_orthognal', bb.c_bool),
        ('_init_coord_length', bb.c_float),
        ('_background', object.VuRenderObject_p),
        ('_render_objects', object.VuRenderObject_p * MAX_RENDER_OBJECTS),
        ('_render_objects_size', bb.c_int)
    ]


VuSVisu_p = ct.POINTER(VuSVisu)
VuSVisu_p_p = ct.POINTER(VuSVisu_p)

# /**
#  * Initialises SVisu
#  * @param orthogonal: If true: an orthogonal camera is used, if false: an perspective camera
#  * @param opt_coord_length=100: sets the super coord length (if <= 0, no coords will be visible)
#  */
# SVisu *svisu_new(bool orthogonal, float opt_coord_length);
vulib.svisu_new.argtypes = [bb.c_bool, bb.c_float]
vulib.svisu_new.restype = VuSVisu_p

# /**  Kills SVisu and all render objects */
# void svisu_kill(SVisu **self_ptr);
vulib.svisu_kill.argtypes = [VuSVisu_p_p]

# void svisu_set_background_color(SVisu *self,
#                                 vec3 top_left_color, vec3 top_right_color,
#                                 vec3 bottom_left_color, vec3 bottom_right_color);
vulib.svisu_set_background_color.argtypes = [VuSVisu_p, mt.vec3, mt.vec3, mt.vec3, mt.vec3]

# /** Clears all render objects (restarts SVisu) */
# void svisu_clear(SVisu *self);
vulib.svisu_clear.argtypes = [VuSVisu_p]

# /**
#  * Shows up the visualisation window.
#  * If a key of quit_at_keys is pressed, the window will close.
#  * @returns: the last pressed key (which might be one of quit_at_keys)
#  */
# char svisu_show(SVisu *self);
vulib.svisu_show.argtypes = [VuSVisu_p]
vulib.svisu_show.restype = bb.c_char

#
# primitives
#

# /**
#  * Shows the coordination system of a pose.
#  */
# void svisu_coords(SVisu *self, mat4 pose, float length,
#                   vec4 color_x, vec4 color_y, vec4 color_z);
vulib.svisu_coords.argtypes = [VuSVisu_p, mt.mat4, bb.c_float,
                               mt.vec4, mt.vec4, mt.vec4]

# /** Shows a plane in svisu */
# VuRawMesh *svisu_plane(SVisu *self, mat4 pose, float width, float height, vec4 color);
vulib.svisu_plane.argtypes = [VuSVisu_p, mt.mat4, bb.c_float, bb.c_float, mt.vec4]

# /** Shows a box in svisu */
# VuRawMesh *svisu_box(SVisu *self, mat4 pose, float width, float height, float extend, vec4 color);
vulib.svisu_box.argtypes = [VuSVisu_p, mt.mat4, bb.c_float, bb.c_float, bb.c_float, mt.vec4]

# /** Shows a circle in svisu */
# VuRawMesh *svisu_circle(SVisu *self, mat4 pose, float radius, vec4 color);
vulib.svisu_circle.argtypes = [VuSVisu_p, mt.mat4, bb.c_float, mt.vec4]

# /** Shows a cylinder hull in svisu */
# VuRawMesh *svisu_cylinder_hull(SVisu *self, vec3 start_point, vec3 extend,
#                                float radius, vec4 color);
vulib.svisu_cylinder_hull.argtypes = [VuSVisu_p, mt.vec3, mt.vec3, bb.c_float, mt.vec4]

# /** Shows a cylinder in svisu */
# VuRawMesh *svisu_cylinder(SVisu *self, vec3 start_point, vec3 extend,
#                           float radius, vec4 color);
vulib.svisu_cylinder.argtypes = [VuSVisu_p, mt.vec3, mt.vec3, bb.c_float, mt.vec4]

# /** Shows a cone part hull in svisu */
# VuRawMesh *svisu_cone_part_hull(SVisu *self, vec3 start_point, vec3 extend,
#                                 float start_radius, float end_radius, vec4 color);
vulib.svisu_cone_part_hull.argtypes = [VuSVisu_p, mt.vec3, mt.vec3, bb.c_float, bb.c_float, mt.vec4]

# /** Shows a cone hull in svisu */
# VuRawMesh *svisu_cone_hull(SVisu *self, vec3 apex_point, vec3 extend,
#                            float opening_angle_rad, vec4 color);
vulib.svisu_cone_hull.argtypes = [VuSVisu_p, mt.vec3, mt.vec3, bb.c_float, mt.vec4]

# /** Shows a cone in svisu */
# VuRawMesh *svisu_cone(SVisu *self, vec3 apex_point, vec3 extend,
#                       float opening_angle_rad, vec4 color);
vulib.svisu_cone.argtypes = [VuSVisu_p, mt.vec3, mt.vec3, bb.c_float, mt.vec4]

# /** Shows a sphere in svisu */
# VuRawMesh *svisu_sphere(SVisu *self, vec3 mid_pos, float radius, vec4 color);
vulib.svisu_sphere.argtypes = [VuSVisu_p, mt.vec3, bb.c_float, mt.vec4]

# /** Shows a half sphere in svisu */
# VuRawMesh *svisu_half_sphere(SVisu *self, mat4 pose, float radius, vec4 color);
vulib.svisu_half_sphere.argtypes = [VuSVisu_p, mt.mat4, bb.c_float, mt.vec4]

# /** Shows an arrow in svisu */
# VuRawMesh *svisu_arrow(SVisu *self, vec3 apex_point, vec3 extend,
#                        float radius, vec4 color);
vulib.svisu_arrow.argtypes = [VuSVisu_p, mt.vec3, mt.vec3, bb.c_float, mt.vec4]

# /** Shows an arrow in svisu */
# VuRawMesh *svisu_arrow_swapped(SVisu *self, vec3 end_point, vec3 extend,
#                                float radius, vec4 color);
vulib.svisu_arrow_swapped.argtypes = [VuSVisu_p, mt.vec3, mt.vec3, bb.c_float, mt.vec4]


class SVisu:
    def __init__(self, orthogonal: bool, opt_coord_length: float = 100.0):
        """
        Initialises SVisu.

        :param orthogonal: If true: an orthogonal camera is used, if false: an perspective camera
        :param opt_coord_length: sets the base coord length (if <= 0, no coords will be visible)
        """
        self._impl = vulib.svisu_new(orthogonal, opt_coord_length)
        self._mouse_ray_callback = None
        self._render_callback = None
        self._trackbar_callbacks = []
        self._trackbar_callback_user_data = []
        self._render_objects = []
        self.visu = visubase.VisuBase(self._impl.contents.visu)

    def __del__(self):
        vulib.svisu_kill(bb.ref(self._impl))
        self._impl = None

    def set_mouse_ray_callback(self, callback, callback_user_data=None):
        def mouse_ray_callback(sv: bb.c_void_p, ray_pos: mt.vec3, ray_dir: mt.vec3, user_data: bb.c_void_p):
            ray_pos_array = mt.cast_from_vec3(ray_pos)
            ray_dir_array = mt.cast_from_vec3(ray_dir)
            callback(self, ray_pos_array, ray_dir_array, callback_user_data)

        self._mouse_ray_callback = svisu_mouse_ray_callback_fun(mouse_ray_callback)
        self._impl.contents.mouse_ray_callback = self._mouse_ray_callback

    def set_render_callback(self, callback, callback_user_data=None):
        def render_callback(sv: bb.c_void_p, user_data: bb.c_void_p):
            callback(self, callback_user_data)

        self._render_callback = svisu_render_callback_fun(render_callback)
        self._impl.contents.render_callback = self._render_callback

    def add_trackbar(self,
                     name: str,
                     min: float,
                     max: float,
                     step: float,
                     init_value: float,
                     callback,
                     callback_user_data=None):
        """
        Adds a trackbar to the top of SVisu.

        :param callback: Function in the form of def cb(svisu, value, user_data)
        """
        if self._impl.contents.trackbars_size >= MAX_TRACKBARS:
            raise RuntimeError('Too many trackbars')
        tb = self._impl.contents.trackbars[self._impl.contents.trackbars_size]
        tb.name = name.encode()
        tb.min = min
        tb.max = max
        tb.step = step
        tb.init_value = init_value

        def trackbar_callback(sv: VuSVisu_p, value: bb.c_double, user_data: bb.c_void_p):
            id = user_data - 1
            callback(self, value, self._trackbar_callback_user_data[id])

        TB_CALLBACK_FUNCTION = ct.CFUNCTYPE(None, VuSVisu_p, bb.c_double, bb.c_void_p)
        tb_callback = ct.cast(TB_CALLBACK_FUNCTION(trackbar_callback), ct.CFUNCTYPE(None))
        self._trackbar_callbacks.append(tb_callback)  # prevent deallaction
        self._trackbar_callback_user_data.append(callback_user_data)

        tb.callback = tb_callback
        tb.callback_user_data = self._impl.contents.trackbars_size + 1

        self._impl.contents.trackbars_size += 1

    def append_render_object(self, render_object: object.RenderObject):
        if self._impl.contents._render_objects_size >= MAX_RENDER_OBJECTS:
            raise RuntimeError('Too many render objects')
        self._impl.contents._render_objects[self._impl.contents._render_objects_size] = render_object._impl
        self._impl.contents._render_objects_size += 1
        self._render_objects.append(render_object)

    def set_background_color(self,
                             top_left_color: mt.Array,
                             top_right_color: mt.Array,
                             bottom_left_color: mt.Array,
                             bottom_right_color: mt.Array):
        vulib.svisu_set_background_color(self._impl,
                                         mt.cast_array_vec3(top_left_color),
                                         mt.cast_array_vec3(top_right_color),
                                         mt.cast_array_vec3(bottom_left_color),
                                         mt.cast_array_vec3(bottom_right_color))

    def clear(self):
        """
        Clears all render objects (restarts SVisu)
        """
        vulib.svisu_clear(self._impl)
        self._render_objects.clear()

    def show(self) -> str:
        """
        Shows up the visualisation window.
        If a key of quit_at_keys is pressed, the window will close.

        :return: the last pressed key (which might be one of quit_at_keys)
        """
        key = vulib.svisu_show(self._impl)
        return key.decode()

    #
    # primitives
    #

    def coords(self,
               pose: mt.Array,
               length: float,
               color_x: mt.Array = YELLOW,
               color_y: mt.Array = CYAN,
               color_z: mt.Array = MAGENTA):
        """
        Shows the coordination system of a pose.
        """
        vulib.svisu_coords(self._impl,
                           mt.cast_array_mat4(pose),
                           length,
                           mt.cast_array_vec4(color_x),
                           mt.cast_array_vec4(color_y),
                           mt.cast_array_vec4(color_z))

    def plane(self,
              pose: mt.Array,
              width: float,
              height: float,
              color: mt.Array = WHITE):
        """
        Shows a plane in svisu
        """
        vulib.svisu_plane(self._impl,
                          mt.cast_array_mat4(pose),
                          width,
                          height,
                          mt.cast_array_vec4(color))

    def box(self,
            pose: mt.Array,
            width: float,
            height: float,
            extend: float,
            color: mt.Array = WHITE):
        """
        Shows a box in svisu
        """
        vulib.svisu_box(self._impl,
                        mt.cast_array_mat4(pose),
                        width,
                        height,
                        extend,
                        mt.cast_array_vec4(color))

    def circle(self,
               pose: mt.Array,
               radius: float,
               color: mt.Array = WHITE):
        """
        Shows a circle in svisu
        """
        vulib.svisu_circle(self._impl,
                           mt.cast_array_mat4(pose),
                           radius,
                           mt.cast_array_vec4(color))

    def cylinder_hull(self,
                      start_point: mt.Array,
                      extend: mt.Array,
                      radius: float,
                      color: mt.Array = WHITE):
        """
        Shows a cylinder hull in svisu
        """
        vulib.svisu_cylinder_hull(self._impl,
                                  mt.cast_array_vec3(start_point),
                                  mt.cast_array_vec3(extend),
                                  radius,
                                  mt.cast_array_vec4(color))

    def cylinder(self,
                 start_point: mt.Array,
                 extend: mt.Array,
                 radius: float,
                 color: mt.Array = WHITE):
        """
        Shows a cylinder in svisu
        """
        vulib.svisu_cylinder(self._impl,
                             mt.cast_array_vec3(start_point),
                             mt.cast_array_vec3(extend),
                             radius,
                             mt.cast_array_vec4(color))

    def cone_part_hull(self,
                       start_point: mt.Array,
                       extend: mt.Array,
                       start_radius: float,
                       end_radius: float,
                       color: mt.Array = WHITE):
        """
        Shows a cone part hull in svisu
        """
        vulib.svisu_cone_part_hull(self._impl,
                                   mt.cast_array_vec3(start_point),
                                   mt.cast_array_vec3(extend),
                                   start_radius,
                                   end_radius,
                                   mt.cast_array_vec4(color))

    def cone_hull(self,
                  apex_point: mt.Array,
                  extend: mt.Array,
                  opening_angle_rad: float,
                  color: mt.Array = WHITE):
        """
        Shows a cone hull in svisu
        """
        vulib.svisu_cone_hull(self._impl,
                              mt.cast_array_vec3(apex_point),
                              mt.cast_array_vec3(extend),
                              opening_angle_rad,
                              mt.cast_array_vec4(color))

    def cone(self,
             apex_point: mt.Array,
             extend: mt.Array,
             opening_angle_rad: float,
             color: mt.Array = WHITE):
        """
        Shows a cone in svisu
        """
        vulib.svisu_cone(self._impl,
                         mt.cast_array_vec3(apex_point),
                         mt.cast_array_vec3(extend),
                         opening_angle_rad,
                         mt.cast_array_vec4(color))

    def sphere(self,
               mid_pos: mt.Array,
               radius: float,
               color: mt.Array = WHITE):
        """
        Shows a sphere in svisu
        """
        vulib.svisu_sphere(self._impl,
                           mt.cast_array_vec3(mid_pos),
                           radius,
                           mt.cast_array_vec4(color))

    def half_sphere(self,
                    pose: mt.Array,
                    radius: float,
                    color: mt.Array = WHITE):
        """
        Shows a half sphere in svisu
        """
        vulib.svisu_half_sphere(self._impl,
                                mt.cast_array_mat4(pose),
                                radius,
                                mt.cast_array_vec4(color))

    def arrow(self,
              apex_point: mt.Array,
              extend: mt.Array,
              radius: float,
              color: mt.Array = WHITE):
        """
        Shows an arrow in svisu
        """
        vulib.svisu_arrow(self._impl,
                          mt.cast_array_vec3(apex_point),
                          mt.cast_array_vec3(extend),
                          radius,
                          mt.cast_array_vec4(color))

    def arrow_swapped(self,
                      end_point: mt.Array,
                      extend: mt.Array,
                      radius: float,
                      color: mt.Array = WHITE):
        """
        Shows an arrow in svisu
        """
        vulib.svisu_arrow_swapped(self._impl,
                                  mt.cast_array_vec3(end_point),
                                  mt.cast_array_vec3(extend),
                                  radius,
                                  mt.cast_array_vec4(color))

    #
    # objects
    #

    def points(self,
               points: np.ndarray,
               color: mt.Array = WHITE,
               point_size: float = 1) \
            -> object.Points:
        """
        Shows a point cloud with single color.

        :return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
        """
        ro = object.Points(self.visu, points, color, point_size)
        self.append_render_object(ro)
        return ro

    def points_colored(self,
                       points: np.ndarray,
                       colors: np.ndarray,
                       point_size: float = 1) \
            -> object.PointsColored:
        """
        Shows a point cloud with a color for each point.

        :return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
        """
        ro = object.PointsColored(self.visu, points, colors, point_size)
        self.append_render_object(ro)
        return ro

    def point_normals(self,
                      points: np.ndarray,
                      normals: np.ndarray,
                      color: mt.Array = WHITE,
                      point_size: float = 1) \
            -> object.PointNormals:
        """
        Shows a point cloud rendered with normals and a single color

        :return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
        """
        ro = object.PointNormals(self.visu, points, normals, color, point_size)
        self.append_render_object(ro)
        return ro

    def point_normals_colored(self,
                              points: np.ndarray,
                              normals: np.ndarray,
                              colors: np.ndarray,
                              point_size: float = 1) \
            -> object.PointNormalsColored:
        """
        Shows a point cloud rendered with normals and with a color for each point

        :return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
        """
        ro = object.PointNormalsColored(self.visu, points, normals, colors, point_size)
        self.append_render_object(ro)
        return ro

    def lines(self,
              points: np.ndarray,
              color: mt.Array = WHITE,
              line_size: float = 1,
              line_mode: int = object.MODE_LINES) \
            -> object.Lines:
        """
        Shows lines with colors for each point

        :return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
        """
        ro = object.Lines(self.visu, points, color, line_size, line_mode)
        self.append_render_object(ro)
        return ro

    def lines_colored(self,
                      points: np.ndarray,
                      colors: np.ndarray,
                      line_size: float = 1,
                      line_mode: int = object.MODE_LINES) \
            -> object.LinesColored:
        """
        Shows lines with colors for each point

        :return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
        """
        ro = object.LinesColored(self.visu, points, colors, line_size, line_mode)
        self.append_render_object(ro)
        return ro

    def mesh(self,
             points: np.ndarray,
             indices: np.ndarray,
             color: mt.Array = WHITE) \
            -> object.Mesh:
        """
        Shows a mesh with single color

        :return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
        """
        ro = object.Mesh(self.visu, points, indices, color)
        self.append_render_object(ro)
        return ro

    def mesh_colored(self,
                     points: np.ndarray,
                     colors: np.ndarray,
                     indices: np.ndarray) \
            -> object.MeshColored:
        """
        Shows a mesh with color for each point

        :return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
        """
        ro = object.MeshColored(self.visu, points, colors, indices)
        self.append_render_object(ro)
        return ro

    def mesh_colored_matt(self,
                          points: np.ndarray,
                          colors: np.ndarray,
                          indices: np.ndarray) \
            -> object.MeshColoredMatt:
        """
        Shows a mesh with color for each point, without normal shining

        :return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
        """
        ro = object.MeshColoredMatt(self.visu, points, colors, indices)
        self.append_render_object(ro)
        return ro

    def raw_mesh(self,
                 points: np.ndarray,
                 normals: np.ndarray,
                 color: mt.Array = WHITE) \
            -> object.RawMesh:
        """
        Shows a mesh with expanded point normals and single color

        :return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
        """
        ro = object.RawMesh(self.visu, points, normals, color)
        self.append_render_object(ro)
        return ro

    def raw_mesh_colored(self,
                         points: np.ndarray,
                         normals: np.ndarray,
                         colors: np.ndarray) \
            -> object.RawMeshColored:
        """
        Shows a mesh with expanded point normal colors

        :return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
        """
        ro = object.RawMeshColored(self.visu, points, normals, colors)
        self.append_render_object(ro)
        return ro
