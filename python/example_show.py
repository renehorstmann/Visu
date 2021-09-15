import pbase as p
import visu as vu
if __name__ == '__main__':
    points, indices = p.io.load_mesh_stl('../tests/data/test_io_stl_giraffebinary.stl')
    sv = vu.SVisu(True, 10)
    sv.mesh(points, indices)    # color=vu.WHITE as default
    sv.sphere([10, 10, 75], 10, [1, 0, 0, 0.75])
    sv.show()
