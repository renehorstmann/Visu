#ifndef VISU_OBJECT_MESH_H
#define VISU_OBJECT_MESH_H

#include <pbase/types/cloud.h>
#include <pbase/types/meshindices.h>
#include "base.h"

enum vu_meshmode {
    VU_MESH_MODE_SURFACES,
    VU_MESH_MODE_WIREFRAME,
    VU_MESH_MODE_SURFACES_AND_WIREFRAME,
    VU_MESH_MODE_NUM_MODES,
};

/**
 * Render object which renders a mesh with single color.
 */
typedef struct {
    VuRenderObject super;

    pCloud points;
    pMeshIndices indices;
    vec4 color;
    vec4 wire_color;
    float wire_size;
    enum vu_meshmode mesh_mode;
    VuLightOptions_s light_options;
} VuMesh;

/**
 * Creates a new render object (new + init).
 * The return object is the super class for all render objects, it can be cast to the desired form.
 * Render it in visus render_function with vu_render_object_render.
 *
 * wire_color = surface_color
 * wire_size = 1
 * mesh_mode = VU_MESH_MODE_SURFACES
 */
VuRenderObject *vu_mesh_new(VuVisu *visu, pCloud points, pMeshIndices indices, vec4 color);


/**
 * Render object which renders a mesh with colors for each point.
 */
typedef struct {
    VuRenderObject super;

    pCloud points;
    pCloud colors;
    pMeshIndices indices;
    float wire_size;
    enum vu_meshmode mesh_mode;
    VuLightOptions_s light_options;
} VuMeshColored;

/**
 * Creates a new render object (new + init).
 * The return object is the super class for all render objects, it can be cast to the desired form.
 * Render it in visus render_function with vu_render_object_render.
 *
 * wire_size = 1
 * mesh_mode = VU_MESH_MODE_SURFACES
 */
VuRenderObject *vu_mesh_colored_new(VuVisu *visu, pCloud points, pCloud colors, pMeshIndices indices);


/**
 * Render object which renders a mesh with colors for each point, but without normal shines
 */
typedef struct {
    VuRenderObject super;

    pCloud points;
    pCloud colors;
    pMeshIndices indices;
    float wire_size;
    enum vu_meshmode mesh_mode;
} VuMeshColoredMatt;

/**
 * Creates a new render object (new + init).
 * The return object is the super class for all render objects, it can be cast to the desired form.
 * Render it in visus render_function with vu_render_object_render.
 *
 * wire_size = 1
 * mesh_mode = VU_MESH_MODE_SURFACES
 */
VuRenderObject *vu_mesh_colored_matt_new(VuVisu *visu, pCloud points, pCloud colors, pMeshIndices indices);


/**
 * Render object which renders a mesh with expanded point normals (every 3 points = triangle).
 */
typedef struct {
    VuRenderObject super;

    pCloud points;
    pCloud normals;
    vec4 color;
    vec4 wire_color;
    float wire_size;
    enum vu_meshmode mesh_mode;
    VuLightOptions_s light_options;
} VuRawMesh;

/**
 * Creates a new render object (new + init).
 * The return object is the super class for all render objects, it can be cast to the desired form.
 * Render it in visus render_function with vu_render_object_render.
 *
 * wire_color = surface_color
 * wire_size = 1
 * mesh_mode = VU_MESH_MODE_SURFACES
 */
VuRenderObject *vu_raw_mesh_new(VuVisu *visu, pCloud points, pCloud normals, vec4 color);


/**
 * Render object which renders a mesh with expanded point normals colors (every 3 points = triangle).
 */
typedef struct {
    VuRenderObject super;

    pCloud points;
    pCloud normals;
    pCloud colors;
    float wire_size;
    enum vu_meshmode mesh_mode;
    VuLightOptions_s light_options;
} VuRawMeshColored;

/**
 * Creates a new render object (new + init).
 * The return object is the super class for all render objects, it can be cast to the desired form.
 * Render it in visus render_function with vu_render_object_render.
 *
 * wire_size = 1
 * mesh_mode = VU_MESH_MODE_SURFACES
 */
VuRenderObject *vu_raw_mesh_colored_new(VuVisu *visu, pCloud points, pCloud normals, pCloud colors);


#endif //VISU_OBJECT_MESH_H
