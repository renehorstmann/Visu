import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk

from typing import Optional
import visu as vu
import numpy as np

visu: Optional[vu.visubase.Visu] = None
points: Optional[vu.object.RenderObject] = None
visuwidget: Optional[vu.gtk.visuwidget.VisuWidget] = None


def create_window():
    global visu, visuwidget

    window = Gtk.Window()
    window.set_title("main")
    window.set_default_size(600, 400)

    visuwidget = vu.gtk.visuwidget.VisuWidget(visu)
    visuwidget.set_control_callbacks(window)

    visuwidget.glarea.set_hexpand(True)
    visuwidget.glarea.set_vexpand(True)
    window.add(visuwidget.glarea)

    return window


data = np.array([
    [0, 0, 0, 1],
    [10, 0, 0, 1],
    [20, 0, 0, 1],
    [30, 0, 0, 1],
    [0, 20, 0, 1],
    [10, 20, 0, 1],
    [20, 20, 0, 1],
    [30, 20, 0, 1],
    [0, 10, 0, 1],
    [30, 10, 0, 1]
], dtype=np.float32)


def add_object():
    global visu, points
    points = vu.object.points.Points(visu, data, [1, 1, 1, 1], 10)


def render(visu: vu.visubase.Visu, data):
    visu.mode3D()
    points.render()


if __name__ == '__main__':
    print('Start')

    ortho = True
    visu = vu.visubase.Visu(ortho, render, None)

    add_object()

    # gtk_init is called during import
    window = create_window()
    window.connect("destroy", Gtk.main_quit)
    window.show_all()
    Gtk.main()

    print('fin')
