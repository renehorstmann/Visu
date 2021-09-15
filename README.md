# Visu
3D data viewer, written in C with OpenGL

Based on [pBase](https://github.com/renehorstmann/pbase)

## Getting started

### Install
see pbase install section.

### Usage
To use pbase in your cmake project:
```cmake
cmake_minimum_required(VERSION 3.0)
project(visutest C)

set(CMAKE_C_STANDARD 11)

add_executable(main main.c)

if (MINGW) # windows
  include_directories("C:/Libraries/include")
endif()

find_library(PBASE_LIB pbase)
find_library(VISU_LIB visu)
target_link_libraries(main ${PBASE_LIB} ${VISU_LIB})
```

### Example
![img](svisu.png)
[example_show.c](tests/example_show.c) loads an stl mesh with pbase and show it with a transparent sphere
```c
#include <stdio.h>
#include <stdlib.h>
#include <pbase/pbase.h>
#include <visu/svisu.h>

int main() {
    pCloud points;
    pMeshIndices indices;
    if (p_io_load_mesh_stl(&points, &indices, "test_io_stl_giraffebinary.stl")) {
        printf("Failed to load mesh: %s", p_error());
        exit(EXIT_FAILURE);
    }

    SVisu *sv = svisu_new(
            true,   // true=orthogonal camera, false=perspective camera
            10      // length of coordination system arrows (<=0 to turn off)
    );
    svisu_mesh(sv, points, indices, SV_WHITE);
    svisu_sphere(sv,
                 (vec3) {{10, 10, 75}},     // mid pos
                 10,                        // radius
                 (vec4) {{1, 0, 0, 0.5}}    // color rgba
    );

    // will open a window to show the mesh and sphere
    svisu_show(sv);

    svisu_kill(&sv);
    p_cloud_kill(&points);
    p_mesh_indices_kill(&indices);
    exit(EXIT_SUCCESS);
}
```

[example_show.py](python/example_show.py) same as above, but in python
```python
import pbase as p
import visu as vu
if __name__ == '__main__':
    points, indices = p.io.load_mesh_stl('../tests/data/test_io_stl_giraffebinary.stl')
    sv = vu.SVisu(True, 10)
    sv.mesh(points, indices)    # color=vu.WHITE as default
    sv.sphere([10, 10, 75], 10, [1, 0, 0, 0.75])
    sv.show()

```


## Author

René Horstmann

## Licence

This project is licensed under the MIT License - see the LICENSE file for details

