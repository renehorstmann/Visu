#ifndef VISU_OBJECT_BASE_H
#define VISU_OBJECT_BASE_H


#include <stdbool.h>
#include "visu/visubase.h"


static const vec3 VU_LIGHT_SOURCE_INIT = {{-0.5, 0.25, 0.75}};
static const vec3 VU_LIGHT_COLOR_INIT = {{1, 1, 1}};
static const float VU_LIGHT_POWER_INIT = 0.66;
static const vec3 VU_AMBIENT_COLOR_INIT = {{0.33, 0.33, 0.33}};

/** Light options for normal light shaders */
typedef struct {
    vec3 light_source;
    vec3 light_color;
    float light_power;
    vec3 ambient_color;
} VuLightOptions_s;

/**
 * Base class for all render objects, to be rendered in Visu.
 * Every render object has its own pose memory, if one wants to set the pose to another memory location,
 * the pointer used_pose can be set to another pose.
 * If the object has transparent color in it, set transparent = true.
 * If the underlying data buffers should be updated, set update = true.
 * If the user data reference classes, such as VuCloud and VuMeshIndices, should be killed, set kill_user_data = true.
 * on_kill_callback will be called, if the object is killed (normally by Visu, or by vu_RenderObject_delete.
 */
typedef struct VuRenderObject VuRenderObject;

typedef void (*vu_render_object_on_kill_callback_fun)(VuRenderObject *object, void *user_data);

struct VuRenderObject {
    void *impl;
    mat4 pose;
    mat4 *used_pose;   // can be set to another valid pose (mat4)
    bool update;
    bool kill_user_data;
    vu_render_object_on_kill_callback_fun on_kill_callback;
    void *on_kill_callback_user_data;
    VuVisuResource visu_resource;
};


/** @return: The data of a property, of NULL if not available. */
void *vu_render_object_property(VuRenderObject *self, const char *name);

/** @return: the type (string) of a property (or NULL if not available). */
const char *vu_render_object_property_type(const VuRenderObject *self, const char *name);

/** @return: A list (on heap) containing all available property names. (Don't forget to call free(list); ) */
const char **vu_render_object_property_names_as_list_on_heap(const VuRenderObject *self);


/**
 * Kills a render object (says Visu to delete it...).
 * This call is not necessary if Visu will be killed also, (Visu will kill all render objects automatically)
 */
void vu_render_object_kill(VuRenderObject **self_ptr);

/**
 * Uses OpenGL-calls to render the object.
 */
void vu_render_object_render(VuRenderObject *self);


#endif //VISU_OBJECT_BASE_H
