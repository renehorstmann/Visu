#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include <gtk/gtk.h>

#include <pbase/mathc/float.h>

#include "visu/control/mouserotate.h"


#include "visu/gtk/visuwidget.h"
#include "visu/object/points.h"

static VuVisu *visu;
static VuRenderObject *points;
static VuVisuWidget *visuwidget;

static GtkWidget *create_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "main");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);;

    visuwidget = vu_visu_widget_new(visu);
    vu_visu_widget_set_control_callbacks(visuwidget, GTK_WINDOW(window));

    gtk_widget_set_hexpand(visuwidget->glarea, TRUE);
    gtk_widget_set_vexpand(visuwidget->glarea, TRUE);
    gtk_container_add(GTK_CONTAINER(window), visuwidget->glarea);

    return window;
}

static float data[10][4] = {
        {0,  0,  0, 1},
        {10, 0,  0, 1},
        {20, 0,  0, 1},
        {30, 0,  0, 1},
        {0,  20, 0, 1},
        {10, 20, 0, 1},
        {20, 20, 0, 1},
        {30, 20, 0, 1},
        {0,  10, 0, 1},
        {30, 10, 0, 1}
};

static void add_object() {
    pCloud points_cloud = {(vec4*) data, 10};

    points = vu_points_new(visu, points_cloud, vec4_set(1), 10);
}

static void render(VuVisu *vu, void *user_data) {
    vu_visu_3D(vu);
    vu_render_object_render(points);
}

int main() {
    puts("Start");

    bool ortho = true;
    visu = vu_visu_new(ortho, render, NULL);

    add_object();

    gtk_init(NULL, NULL);
    GtkWidget *window = create_window();
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();

    vu_visu_kill(&visu);

    puts("fin");
}
