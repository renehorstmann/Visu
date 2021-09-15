import visu as vu
import numpy as np

if __name__ == '__main__':
    print('visu test...')

    points = np.empty((100, 4), dtype=np.float32)
    points[..., 3] = 1
    points[..., 0] = np.random.uniform(0, 100, 100)
    points[..., 1] = np.random.uniform(0, 40, 100)
    points[..., 2] = np.random.uniform(10, 20, 100)

    arrow = vu.meshprimitives.arrow_swapped((100, 100, 100), (100, 0, 0), 10)

    sv = vu.SVisu(True)
    sv.points(points)
    sv.mesh(*arrow)
    sv.show()

    vu.log.info('hey')

    print('fin')
