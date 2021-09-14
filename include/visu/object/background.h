#ifndef VISU_OBJECT_BACKGROUND_H
#define VISU_OBJECT_BACKGROUND_H


#include "base.h"


static const vec3 VU_BACKGROUND_DEFAULT_TOP_LEFT_COLOR = {{0.1, 0.1, 0.6}};
static const vec3 VU_BACKGROUND_DEFAULT_TOP_RIGHT_COLOR = {{0.1, 0.1, 0.6}};
static const vec3 VU_BACKGROUND_DEFAULT_BOTTOM_LEFT_COLOR = {{0.1, 0.1, 0.4}};
static const vec3 VU_BACKGROUND_DEFAULT_BOTTOM_RIGHT_COLOR = {{0.1, 0.1, 0.4}};

/**
 * Creates a background to render in non uniform 2D mode.
 */
VuRenderObject *vu_create_background(VuVisu *visu,
                                     vec3 top_left_color, vec3 top_right_color,
                                     vec3 bottom_left_color, vec3 bottom_right_color);


#endif //VISU_OBJECT_BACKGROUND_H
