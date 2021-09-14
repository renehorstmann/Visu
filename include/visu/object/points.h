#ifndef VISU_OBJECT_POINTS_H
#define VISU_OBJECT_POINTS_H

#include <pbase/types/cloud.h>
#include "base.h"

/**
 * Render object which renders a point cloud, without normals and a single color.
 */
typedef struct {
    VuRenderObject super;

    pCloud points;
    vec4 color;
    float point_size;
} VuPoints;

/**
 * Creates a new render object (new + init).
 * The return object is the super class for all render objects, it can be cast to the desired form.
 * Render it in visus render_function with vu_render_object_render.
 */
VuRenderObject *vu_points_new(VuVisu *visu, pCloud points, vec4 color, float point_size);


/**
 * Render object which renders a point cloud, without normals but with colors for each point.
 */
typedef struct {
    VuRenderObject super;

    pCloud points;
    pCloud colors;
    float point_size;
} VuPointsColored;

/**
 * Creates a new render object (new + init).
 * The return object is the super class for all render objects, it can be cast to the desired form.
 * Render it in visus render_function with vu_render_object_render.
 */
VuRenderObject *vu_points_colored_new(VuVisu *visu, pCloud points, pCloud colors, float point_size);


/**
 * Render object which renders a point cloud, with normals and a single color.
 */
typedef struct {
    VuRenderObject super;

    pCloud points;
    pCloud normals;
    vec4 color;
    float point_size;
    VuLightOptions_s light_options;
} VuPointNormals;

/**
 * Creates a new render object (new + init).
 * The return object is the super class for all render objects, it can be cast to the desired form.
 * Render it in visus render_function with vu_render_object_render.
 */
VuRenderObject *vu_point_normals_new(VuVisu *visu, pCloud points, pCloud normals,
                                     vec4 color, float point_size);

/**
 * Render object which renders a point cloud, with normals and with colors for each point.
 */
typedef struct {
    VuRenderObject super;

    pCloud points;
    pCloud normals;
    pCloud colors;
    float point_size;
    VuLightOptions_s light_options;
} VuPointNormalsColored;

/**
 * Creates a new render object (new + init).
 * The return object is the super class for all render objects, it can be cast to the desired form.
 * Render it in visus render_function with vu_render_object_render.
 */
VuRenderObject *vu_point_normals_colored_new(VuVisu *visu, pCloud points, pCloud normals,
                                             pCloud colors, float point_size);


#endif //VISU_OBJECT_POINTS_H
