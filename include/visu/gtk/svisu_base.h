#ifndef VISU_GTK_SVISU_BASE_H
#define VISU_GTK_SVISU_BASE_H


#include <stdlib.h>
#include <assert.h>

#include "visu/visubase.h"
#include "visuwidget.h"

// The name SVisu has no extra vu_ namespace, because its unusual enough...

#define SVISU_MAX_QUIT_AT_KEYS 8
#define SVISU_MAX_TRACKBARS 8
#define SVISU_MAX_RENDER_OBJECTS 1024


struct VuRenderObject;
typedef struct SVisu SVisu;

/**
 * All needed information to set up a trackbar in SVisu
 */
typedef struct SVisuTrackbar SVisuTrackbar;
typedef void (*svisu_trackbar_callback_fun)(SVisu *sv, double value, void *callback_user_data);


struct SVisuTrackbar {
    const char *name;
    double min, max, step;
    double init_value;

    svisu_trackbar_callback_fun callback;

    void *callback_user_data;
};



typedef void (*svisu_mouse_ray_callback_fun)(SVisu *sv, vec3 ray_pos, vec3 ray_dir, void *user_data);

typedef void (*svisu_render_callback_fun)(SVisu *sv, void *user_data);

/**
 * SVisu is an easy to use visualisation class for point clouds, meshs and other primitives.
 *
 * Control the size and position of the visualisation window with wnd_...
 *
 * Set up to SVISU_MAX_QUIT_AT_KEYS keys on which the window will also close.
 *
 * Add a mouse ray callback with mouse_ray_callback (and _user_data),
 *      the callback is called when the right mouse button is clicked on the screen.
 *
 * Add up to SVISU_MAX_TRACKBARS trackbars on top of the visualisation window.
 */
struct SVisu {
    int wnd_size_x, wnd_size_y;
    int wnd_move_x, wnd_move_y;
    char quit_at_keys[SVISU_MAX_QUIT_AT_KEYS];

    svisu_mouse_ray_callback_fun mouse_ray_callback;
    void *mouse_ray_callback_user_data;

    svisu_render_callback_fun render_callback;
    void *render_callback_user_data;

    SVisuTrackbar trackbars[SVISU_MAX_TRACKBARS];
    int trackbars_size;

    VuVisu *visu;

    // private
    char _last_pressed_key;
    bool _init_as_orthognal;
    float _init_coord_length;
    struct VuRenderObject *_background;
    struct VuRenderObject *_render_objects[SVISU_MAX_RENDER_OBJECTS];
    int _render_objects_size;
};

/**
 * Initialises SVisu
 * @param orthogonal: If true: an orthogonal camera is used, if false: an perspective camera
 * @param opt_coord_length=100: sets the super coord length (if <= 0, no coords will be visible)
 */
SVisu *svisu_new(bool orthogonal, float opt_coord_length);

/**  Kills SVisu and all render objects */
void svisu_kill(SVisu **self_ptr);

/**
 * Adds a trackbar to the top of SVisu.
 */
static void svisu_add_trackbar(SVisu *self, SVisuTrackbar trackbar) {
    assert(self->trackbars_size < SVISU_MAX_TRACKBARS);
    self->trackbars[self->trackbars_size++] = trackbar;
}

static void svisu_append_render_object(SVisu *self, struct VuRenderObject *render_object) {
    assert(self->_render_objects_size < SVISU_MAX_RENDER_OBJECTS);
    self->_render_objects[self->_render_objects_size++] = render_object;
}

void svisu_set_background_color(SVisu *self,
                                vec3 top_left_color, vec3 top_right_color,
                                vec3 bottom_left_color, vec3 bottom_right_color);

/** Clears all render objects (restarts SVisu) */
void svisu_clear(SVisu *self);

/**
 * Shows up the visualisation window.
 * If a key of quit_at_keys is pressed, the window will close.
 * @returns: the last pressed key (which might be one of quit_at_keys)
 */
char svisu_show(SVisu *self);


#endif //VISU_GTK_SVISU_BASE_H
