#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stddef.h>

#include <pbase/mathc/mathc.h>
#include <pbase/meshprimitives.h>
#include "visu/visu.h"


int main() {
    puts("Start");


    SVisu *sv = svisu_new(true, 0);
    svisu_sphere(sv, (vec3) {{100, 0, 0}}, 50, SV_WHITE);

    pCloud points, normals;
    p_meshprimitives_raw_cone_part_hull(&points, &normals,
                                        (vec3) {{0, 100, 100}},
                                        (vec3) {{0, 0, -100}},
                                        0, 100, 100);
    svisu_raw_mesh(sv, points, normals, SV_RED);

    pCloud normal_lines = p_cloud_new_empty(points.size * 2);
    for(int i=0; i<points.size; i++) {
        vec4 *a = &normal_lines.data[i*2];
        vec4 *b = &normal_lines.data[i*2+1];

        *a = points.data[i];
        *b = vec4_add_vec(*a, vec4_scale(normals.data[i], 10));
    }

    svisu_lines(sv, normal_lines, SV_GREEN, 1, VU_LINE_MODE_LINES);


    puts("1");
    svisu_show(sv);

    puts("2");
    svisu_show(sv);

    svisu_kill(&sv);

    puts("fin");
}
