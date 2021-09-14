#include <pbase/mathc/float.h>
#include "visu/object/mesh.h"

#include "visu/object/background.h"


VuRenderObject *vu_create_background(VuVisu *visu,
                                     vec3 top_left_color, vec3 top_right_color,
                                     vec3 bottom_left_color, vec3 bottom_right_color) {
    pCloud points = p_cloud_new_empty(4);
    pCloud colors = p_cloud_new_zeros_1(4);

    points.data[0] = (vec4) {{-1, +1, 0, 1}};
    points.data[1] = (vec4) {{+1, +1, 0, 1}};
    points.data[2] = (vec4) {{-1, -1, 0, 1}};
    points.data[3] = (vec4) {{+1, -1, 0, 1}};

    colors.data[0].rgb = top_left_color;
    colors.data[1].rgb = top_right_color;
    colors.data[2].rgb = bottom_left_color;
    colors.data[3].rgb = bottom_right_color;

    pMeshIndices indices = p_mesh_indices_new_empty(2);
    indices.data[0] = (ivec3) {{0, 2, 3}};
    indices.data[1] = (ivec3) {{0, 3, 1}};

    VuRenderObject *background = vu_mesh_colored_matt_new(visu, points, colors, indices);
    background->kill_user_data = true;
    return background;
}
