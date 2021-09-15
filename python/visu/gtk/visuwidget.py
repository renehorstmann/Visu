import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk

from .. import log
from .. import visubase

PRELOG = '[Visu::VisuWidget] '

class VisuWidget:
    """
    GTK+ (pygobject) widget, that implements Visu
    """

    def __init__(self, visu: visubase.Visu):
        """
        Creates the widget
        :param visu: The Visu OpenGL implementation
        """
        self.visu = visu
        self.glarea = Gtk.GLArea()
        self.glarea.set_required_version(4, 2)
        self.glarea.set_has_depth_buffer(True)

        self.glarea.connect('realize', self._realize)
        self.glarea.connect('unrealize', self._unrealize)
        self.glarea.connect('render', self._render)

    def set_control_callbacks(self, window: Gtk.Window):
        window.add_events(Gdk.EventMask.KEY_PRESS_MASK
                          | Gdk.EventMask.KEY_RELEASE_MASK)
        window.connect('key_press_event', self._key_callback)
        window.connect('key_release_event', self._key_callback)

        self.glarea.add_events(Gdk.EventMask.BUTTON_PRESS_MASK
                               | Gdk.EventMask.BUTTON_RELEASE_MASK
                               | Gdk.EventMask.POINTER_MOTION_MASK
                               | Gdk.EventMask.SCROLL_MASK)
        self.glarea.connect('button_press_event', self._mouse_click_callback)
        self.glarea.connect('button_release_event', self._mouse_click_callback)
        self.glarea.connect('motion_notify_event', self._mouse_move_callback)
        self.glarea.connect('scroll_event', self._mouse_scroll_callback)

    def _realize(self, glarea):
        """
        will be called, when the OpenGL context is created in this widget
        """
        log.info(PRELOG+'realize')
        self.glarea.make_current()
        self.visu.realize()

    def _unrealize(self, glarea):
        """
        will be called, when the OpenGL context is lost in this widget
        """
        log.info(PRELOG+'unrealize')
        self.glarea.make_current()
        self.visu.unrealize()

    def _render(self, glarea, context):
        """
        will be called, to render in Visu
        """
        self.glarea.make_current()
        self.visu.render(self.glarea.get_allocated_width(), self.glarea.get_allocated_height())
        self.glarea.queue_draw()

    def _key_callback(self, window, event):
        opt_control = self.visu.get_control()
        if opt_control is None:
            return False
        pressed = event.type == Gdk.EventType.KEY_PRESS
        key = event.keyval
        opt_control.key_event(key, pressed)
        return False  # let other callbacks catch key events

    def _mouse_click_callback(self, window, event):
        opt_control = self.visu.get_control()
        if opt_control is None:
            return False
        clicked = event.type == Gdk.EventType.BUTTON_PRESS
        opt_control.mouse_click_event(event.x, event.y, event.button, clicked)
        return False  # let other callbacks catch key events

    def _mouse_move_callback(self, window, event):
        opt_control = self.visu.get_control()
        if opt_control is None:
            return False
        opt_control.mouse_move_event(event.x, event.y)
        return False  # let other callbacks catch key events

    def _mouse_scroll_callback(self, window, event):
        opt_control = self.visu.get_control()
        if opt_control is None:
            return False
        if event.direction != Gdk.ScrollDirection.UP and event.direction != Gdk.ScrollDirection.DOWN:
            return False
        up = event.direction == Gdk.ScrollDirection.UP
        opt_control.mouse_scroll_event(up)
        return False  # let other callbacks catch key events
