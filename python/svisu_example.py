import numpy as np
import visu as vu

PRELOG='[svisu_example] '

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


def mouse_ray_callback(sv, ray_pos, ray_dir, user_data):
    print('ray!\n - pos{%f, %f, %f}\n - dir{%f %f %f}' %
          (ray_pos[0], ray_pos[1], ray_pos[2],
           ray_dir[0], ray_dir[1], ray_dir[2]))


render_cnts = 0


def render_callback(sv, user_data):
    global render_cnts
    render_cnts += 1
    if render_cnts % 60 == 0:
        print('render!')


def trackbar_callback(sv, value, user_data):
    print('tracky! value:', value)


if __name__ == '__main__':
    print('Start')

    vu.log.trace(PRELOG+'log_test')

    sv = vu.SVisu(False, 10)
    sv.set_background_color(vu.WHITE, vu.GREEN, vu.BLUE, vu.RED)

    sv.set_mouse_ray_callback(mouse_ray_callback)

    sv.set_render_callback(render_callback)

    sv.add_trackbar('tracky', 0, 100, 2, 50, trackbar_callback)

    sv.points(data, [1, 0, 1, 1], 10)

    sv.sphere([300, 0, 0], 50, vu.GREEN)
    sv.arrow_swapped([-300, 0, 0], [100, 0, 0], 2, vu.CYAN)

    key = sv.show()
    print('key:', key)

    print('fin')
