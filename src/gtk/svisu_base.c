#include <gtk/gtk.h>

#include <pbase/rhc/log.h>
#include <pbase/rhc/allocator.h>
#include <pbase/mathc/float.h>
#include "visu/control/keyflow.h"
#include "visu/control/mouserotate.h"
#include "visu/object/base.h"
#include "visu/object/mesh.h"
#include "visu/object/background.h"

#include "visu/gtk/svisu_base.h"

#define PRELOG "[SVisu] "

// prototype from svisu_primitives.h
void svisu_coords(SVisu *self, mat4 pose, float length,
                  vec4 color_x, vec4 color_y, vec4 color_z);

struct TrackbarChangedData {
    SVisu *sv;
    SVisuTrackbar *trackbar;
};

static gboolean mouse_click_callback(GtkWidget *widget, GdkEvent *event, gpointer data);

static gboolean key_callback(GtkWidget *widget, GdkEvent *event, gpointer data);

static void trackbar_value_changed(GtkRange *range, gpointer user_data);

static void render_function(VuVisu *visu, void *user_data);

SVisu *svisu_new(bool orthogonal, float opt_coord_length) {
    SVisu *self = p_rhc_malloc(sizeof *self);
    *self = (SVisu) {600, 400,
            ._init_as_orthognal = orthogonal,
            ._init_coord_length = opt_coord_length};

    self->visu = vu_visu_new(orthogonal, render_function, self);

    if (opt_coord_length > 0) {
        svisu_coords(self, mat4_eye(), opt_coord_length,
                     (vec4) {{1, 0, 0, 1}},
                     (vec4) {{0, 1, 0, 1}},
                     (vec4) {{0, 0, 1, 1}}
        );
    }

    svisu_set_background_color(self,
                               VU_BACKGROUND_DEFAULT_TOP_LEFT_COLOR,
                               VU_BACKGROUND_DEFAULT_TOP_RIGHT_COLOR,
                               VU_BACKGROUND_DEFAULT_BOTTOM_LEFT_COLOR,
                               VU_BACKGROUND_DEFAULT_BOTTOM_RIGHT_COLOR);

    return self;
}

void svisu_kill(SVisu **self_ptr) {
    if (*self_ptr) {
        vu_visu_kill(&(*self_ptr)->visu);
    }
    p_rhc_free(*self_ptr);
    *self_ptr = NULL;
}

void svisu_set_background_color(SVisu *self,
                                vec3 top_left_color, vec3 top_right_color,
                                vec3 bottom_left_color, vec3 bottom_right_color) {
    if (self->_background)
        vu_render_object_kill(&self->_background);

    self->_background = vu_create_background(self->visu,
                                             top_left_color, top_right_color,
                                             bottom_left_color, bottom_right_color);
}

void svisu_clear(SVisu *self) {
    vu_visu_kill(&self->visu);
    SVisu *tmp = svisu_new(self->_init_as_orthognal, self->_init_coord_length);

    //move
    *self = *tmp;
    p_rhc_free(tmp);
}

char svisu_show(SVisu *self) {
    // signals ? (signal(SIGINT, SIG_DFL))

    gtk_init(NULL, NULL);

    // window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "SVisu");
    gtk_window_set_default_size(GTK_WINDOW(window), self->wnd_size_x, self->wnd_size_y);
    gtk_window_move(GTK_WINDOW(window), self->wnd_move_x, self->wnd_move_y);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // VisuWidget
    VuVisuWidget *visuwidget = vu_visu_widget_new(self->visu);
    vu_visu_widget_set_control_callbacks(visuwidget, GTK_WINDOW(window));

    gtk_widget_set_hexpand(visuwidget->glarea, TRUE);
    gtk_widget_set_vexpand(visuwidget->glarea, TRUE);

    // callbacks
    gtk_widget_add_events(GTK_WIDGET(window), GDK_KEY_PRESS_MASK);
    g_signal_connect (window, "key_press_event", G_CALLBACK(key_callback), self);
    gtk_widget_add_events(visuwidget->glarea, GDK_BUTTON_PRESS_MASK);
    g_signal_connect (visuwidget->glarea, "button_press_event", G_CALLBACK(mouse_click_callback), self);


    // Container vbox
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // add each trackbar
    struct TrackbarChangedData trackbar_data[SVISU_MAX_TRACKBARS];
    for (int i = 0; i < self->trackbars_size; i++) {
        GtkWidget *tbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_box_pack_start(GTK_BOX(vbox), tbox, FALSE, FALSE, 0);

        GtkWidget *name_label = gtk_label_new(self->trackbars[i].name);
        gtk_box_pack_start(GTK_BOX(tbox), name_label, FALSE, FALSE, 5);

        GtkWidget *scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,
                                                    self->trackbars[i].min,
                                                    self->trackbars[i].max,
                                                    self->trackbars[i].step);
        gtk_range_set_value(GTK_RANGE(scale), self->trackbars[i].init_value);
        gtk_box_pack_start(GTK_BOX(tbox), scale, TRUE, TRUE, 5);

        trackbar_data[i] = (struct TrackbarChangedData) {self, &self->trackbars[i]};
        g_signal_connect (scale, "value-changed", G_CALLBACK(trackbar_value_changed), &trackbar_data[i]);
    }

    // add VisuWidget into the box
    gtk_box_pack_start(GTK_BOX(vbox), visuwidget->glarea, TRUE, TRUE, 0);

    // run
    gtk_widget_show_all(window);
    gtk_main();

    return self->_last_pressed_key;
}

static gboolean key_callback(GtkWidget *widget, GdkEvent *event, gpointer data) {
    SVisu *self = data;

    char key = event->key.keyval;
    self->_last_pressed_key = key;

    for (int i = 0; i < SVISU_MAX_QUIT_AT_KEYS; i++) {
        if (key == self->quit_at_keys[i]) {
            log_info("close by key %c", key);
            gtk_window_close(GTK_WINDOW(widget));
            return TRUE;
        }
    }

    // camera home
    if (key == 'h') {
        mat4 home = self->_init_as_orthognal ?
                    VU_MOUSE_ROTATE_CONTROL_INIT_CENTER_POSE : VU_KEY_FLOW_CONTROL_INIT_POSE;
        self->visu->opt_control->set_pose(self->visu->opt_control, home);
    }

    // log camera pose
    if (key == 'p') {
        mat4 pose = self->visu->opt_control->get_pose(self->visu->opt_control);
        const char *info = self->_init_as_orthognal ?
                           "camera center pose:" : "camera pose:";
        log_info("%s\n(float[16]) {\n"
                 "%8.2f, %8.2f, %8.2f, %8.2f,\n"
                 "%8.2f, %8.2f, %8.2f, %8.2f,\n"
                 "%8.2f, %8.2f, %8.2f, %8.2f,\n"
                 "%8.2f, %8.2f, %8.2f, %8.2f\n}",
                 info,
                 pose.v[0], pose.v[1], pose.v[2], pose.v[3],
                 pose.v[4], pose.v[5], pose.v[6], pose.v[7],
                 pose.v[8], pose.v[9], pose.v[10], pose.v[11],
                 pose.v[12], pose.v[13], pose.v[14], pose.v[15]
        );
    }

    // grow point size
    if (key == '+') {
        for (int i = 0; i < self->_render_objects_size; i++) {
            float *points_size = vu_render_object_property(self->_render_objects[i], "point_size");
            if (points_size)
                (*points_size)++;
        }
    }

    // shrink point size
    if (key == '-') {
        for (int i = 0; i < self->_render_objects_size; i++) {

            float *points_size = vu_render_object_property(self->_render_objects[i], "point_size");
            if (points_size) {
                (*points_size)--;
                if (*points_size < 1)
                    *points_size = 1;
            }
        }
    }

    return FALSE;
}

static gboolean mouse_click_callback(GtkWidget *widget, GdkEvent *event, gpointer data) {
    SVisu *self = data;
    if (self->mouse_ray_callback && event->type == GDK_BUTTON_PRESS && event->button.button == 3) {
        vec3 pos, dir;
        self->visu->camera->position_to_ray(self->visu->camera, &pos, &dir, event->button.x, event->button.y);
        self->mouse_ray_callback(self, pos, dir, self->mouse_ray_callback_user_data);
    }
    return FALSE;
}

static void trackbar_value_changed(GtkRange *range, gpointer user_data) {
    struct TrackbarChangedData *trackbar_data = user_data;
    double value = gtk_range_get_value(range);
    trackbar_data->trackbar->callback(trackbar_data->sv, value, trackbar_data->trackbar->callback_user_data);
}


static void render_function(VuVisu *visu, void *user_data) {
    SVisu *self = user_data;

    vu_visu_2D(visu, false);
    vu_render_object_render(self->_background);

    vu_visu_3D(visu);
    for (int i = 0; i < self->_render_objects_size; i++)
        vu_render_object_render(self->_render_objects[i]);

    if (self->render_callback)
        self->render_callback(self, self->render_callback_user_data);
}

