#ifndef VISU_SVISU_H
#define VISU_SVISU_H


/**
 * SVisu consists of the super class (gtk/svisu_base.h) and the implementations for render objects.
 *
 * Minimal example:
 * ```c
 * #include <visu/svisu.h>
 *
 * int main() {
 *
 *     // create SVisu with (true=orthogonal camera, 100 [mm] coord size)
 *     SVisu *sv = svisu_new(true, 100);
 *
 *     // load a sphere into SVisu at x=100, y=100, z=0 with a radius of 50 in yellow color [mm]
 *     svisu_sphere(sv, (vec3) {{100, 100, 0}}, 50, SV_YELLOW.rgb);
 *
 *     // show up SVisu window (gtk window)
 *     svisu_show(sv);
 *
 *     // kill SVisu
 *     svisu_kill(&sv);
 * }
 * ```
 */

#include "gtk/svisu_base.h"
#include "gtk/svisu_object.h"
#include "gtk/svisu_primitives.h"


#endif //VISU_SVISU_H
