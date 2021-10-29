#include <stdio.h>
#include <stdlib.h>
#include <pbase/pbase.h>
#include <visu/svisu.h>

int main() {
    pCloud points;
    pMeshIndices indices;
    if (p_io_load_mesh_stl(&points, &indices, "giraffe.stl")) {
        printf("Failed to load mesh: %s", p_error());
        exit(EXIT_FAILURE);
    }

    SVisu *sv = svisu_new(
            true,   // true=orthogonal camera, false=perspective camera
            10      // length of coordination system arrows (<=0 to turn off)
    );
    svisu_mesh(sv, points, indices, VU_WHITE);
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
