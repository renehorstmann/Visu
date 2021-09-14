#ifndef VISU_OBJECT_LINES_H
#define VISU_OBJECT_LINES_H

#include <pbase/types/cloud.h>
#include "base.h"

enum vu_linemode {
    VU_LINE_MODE_LINES,
    VU_LINE_MODE_LINE_STRIP,
    VU_LINE_MODE_LINE_LOOP,
    VU_LINE_MODE_NUM_MODES,
};

/**
 * Render object which renders lines.
 */
typedef struct {
    VuRenderObject super;

    pCloud points;
    vec4 color;
    float line_size;
    enum vu_linemode line_mode;
} VuLines;

/**
 * Creates a new render object (new + init).
 * The return object is the super class for all render objects, it can be cast to the desired form.
 * Render it in visus render_function with vu_render_object_render.
 */
VuRenderObject *vu_lines_new(VuVisu *visu, pCloud points, vec4 color, float line_size,
                             enum vu_linemode line_mode);


/**
 * Render object which renders lines with colored points.
 */
typedef struct {
    VuRenderObject super;

    pCloud points;
    pCloud colors;
    float line_size;
    enum vu_linemode line_mode;
} VuLinesColored;

/**
 * Creates a new render object (new + init).
 * The return object is the super class for all render objects, it can be cast to the desired form.
 * Render it in visus render_function with vu_render_object_render.
 */
VuRenderObject *vu_lines_colored_new(VuVisu *visu, pCloud points, pCloud colors, float line_size,
                                     enum vu_linemode line_mode);


#endif //VISU_OBJECT_LINES_H
