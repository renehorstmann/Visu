#ifndef VISU_VISUBASE_H
#define VISU_VISUBASE_H


#include "visu/camera/base.h"
#include "visu/control/base.h"

static const int VU_VISU_INIT_MULTISAMPLE = 4;
static const float VU_VISU_INIT_Z_NEAR = 10;
static const float VU_VISU_INIT_Z_FAR = 50000;

typedef struct VuVisuResource VuVisuResource;

typedef void (*vu_visu_resource_unload_fun)(VuVisuResource *self);

typedef void (*vu_visu_resource_kill_fun)(VuVisuResource *self);

struct VuVisuResource {
    void *impl;
    vu_visu_resource_unload_fun unload;
    vu_visu_resource_kill_fun kill;
};

/**
 * OpenGL visualisation implementation to visualize point clouds, meshs and primitives.
 * Needs a valid OpenGL (min 4.2) context.
 * Take a look at VisuWidget for a GTK+3 GlArea Widget implementation.
 * Or use SVisu for a simplified version for debugging.
 */
typedef struct VuVisu VuVisu;

typedef void (*vu_visu_render_fun)(VuVisu *visu, void *user_data);

struct VuVisu {
    void *impl;
    int multisample;
    VuCamera *camera;
    VuControl *opt_control;

    vu_visu_render_fun render_function;
    void *render_function_user_data;

    // set before render_function is called
    double delta_time;
    int screen_width;
    int screen_height;
    mat4 view;
    mat4 projection;
};

VuVisu *vu_visu_new(bool orthogonal,
                   void (*render_function)(VuVisu *visu, void *user_data),
                   void *render_function_user_data);

void vu_visu_kill(VuVisu **self_ptr);

/** Should be called with active OpenGL context to init Visu */
void vu_visu_realize(VuVisu *self);

/** Should be called when loosing the OpenGL context to clear Visu */
void vu_visu_unrealize(VuVisu *self);

/**
 * Should be called with active OpenGL context, to render.
 * This method sets up OpenGL for the call of the user render function (render_function).
 */
void vu_visu_render(VuVisu *self, int width, int height);

/**
 * Sets up 3D mode.
 * (Depth test + camera view, projection)
 */
void vu_visu_3D(VuVisu *self);

/**
 * Sets up 2D mode.
 * (No Depth test + eye view, 2d projection)
 * @param uniform_view: If false: projection mat will be eye.
 * if true: projection mat will be set to guarantee a uniform 1 quad.
 */
void vu_visu_2D(VuVisu *self, bool uniform_view);

/** Adds an resource that visu will manage */
void vu_visu_add_resource(VuVisu *self, VuVisuResource resource);

/** Calls visu to kill an added resource */
void vu_visu_kill_resource(VuVisu *self, VuVisuResource resource);


#endif //VISU_VISUBASE_H
