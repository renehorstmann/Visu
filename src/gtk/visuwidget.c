#include <gtk/gtk.h>

#include <pbase/rhc/log.h>
#include <pbase/rhc/alloc.h>
#include "visu/visubase.h"
#include "visu/gtk/visuwidget.h"

#define PRELOG "[Visu::VisuWidget] "


static void realize(GtkWidget *widget, gpointer data);

static void unrealize(GtkWidget *widget, gpointer data);

static gboolean render(GtkGLArea *glarea, GdkGLContext *context, gpointer data);

static gboolean key_callback(GtkWidget *widget, GdkEvent *event, gpointer data);

static gboolean mouse_click_callback(GtkWidget *widget, GdkEvent *event, gpointer data);

static gboolean mouse_move_callback(GtkWidget *widget, GdkEvent *event, gpointer data);

static gboolean mouse_scroll_callback(GtkWidget *widget, GdkEvent *event, gpointer data);

VuVisuWidget *vu_visu_widget_new(VuVisu *visu) {
    VuVisuWidget *self = p_rhc_calloc(sizeof *self);
    self->visu = visu;

    self->glarea = gtk_gl_area_new();
    gtk_gl_area_set_required_version(GTK_GL_AREA(self->glarea), 4, 2);
    gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(self->glarea), TRUE);

    g_signal_connect (self->glarea, "realize", G_CALLBACK(realize), self);
    g_signal_connect (self->glarea, "unrealize", G_CALLBACK(unrealize), self);
    g_signal_connect (self->glarea, "render", G_CALLBACK(render), self);

    return self;
}

void vu_visu_widget_kill(VuVisuWidget **self_ptr) {
    p_rhc_free(*self_ptr);
    *self_ptr = NULL;
}

void vu_visu_widget_set_control_callbacks(VuVisuWidget *self, GtkWindow *window) {

    gtk_widget_add_events(GTK_WIDGET(window),
                          GDK_KEY_PRESS_MASK
                          | GDK_KEY_RELEASE_MASK);
    g_signal_connect (window, "key_press_event", G_CALLBACK(key_callback), self);
    g_signal_connect (window, "key_release_event", G_CALLBACK(key_callback), self);

    gtk_widget_add_events(self->glarea,
                          GDK_BUTTON_PRESS_MASK
                          | GDK_BUTTON_RELEASE_MASK
                          | GDK_POINTER_MOTION_MASK
                          | GDK_SCROLL_MASK);
    g_signal_connect (self->glarea, "button_press_event", G_CALLBACK(mouse_click_callback), self);
    g_signal_connect (self->glarea, "button_release_event", G_CALLBACK(mouse_click_callback), self);
    g_signal_connect (self->glarea, "motion_notify_event", G_CALLBACK(mouse_move_callback), self);
    g_signal_connect (self->glarea, "scroll_event", G_CALLBACK(mouse_scroll_callback), self);
}

static void realize(GtkWidget *widget, gpointer data) {
    VuVisuWidget *self = data;
    log_info(PRELOG"realize");

    gtk_gl_area_make_current(GTK_GL_AREA (widget));
    GError *err = gtk_gl_area_get_error(GTK_GL_AREA (widget));
    if (err) {
        log_error(PRELOG"realize failed: %s", err->message);
        return;
    }

    vu_visu_realize(self->visu);
}

static void unrealize(GtkWidget *widget, gpointer data) {
    VuVisuWidget *self = data;
    log_info(PRELOG"unrealize");

    gtk_gl_area_make_current(GTK_GL_AREA (widget));
    GError *err = gtk_gl_area_get_error(GTK_GL_AREA (widget));
    if (err) {
        log_error(PRELOG"realize failed: %s", err->message);
        return;
    }

    vu_visu_unrealize(self->visu);
}


static gboolean render(GtkGLArea *glarea, GdkGLContext *context, gpointer data) {
    VuVisuWidget *self = data;

    gtk_gl_area_make_current(glarea);
    GError *err = gtk_gl_area_get_error(glarea);
    if (err) {
        log_error(PRELOG"realize failed: %s", err->message);
        return FALSE;
    }

    int width = gtk_widget_get_allocated_width(GTK_WIDGET(glarea));
    int height = gtk_widget_get_allocated_height(GTK_WIDGET(glarea));

    vu_visu_render(self->visu, width, height);

    gtk_gl_area_queue_render(glarea);

    return TRUE;
}


static gboolean key_callback(GtkWidget *widget, GdkEvent *event, gpointer data) {
    VuVisuWidget *self = data;
    if (self->visu->opt_control == NULL)
        return FALSE;

    bool pressed = event->type == GDK_KEY_PRESS;
    char key = (char) event->key.keyval;
    self->visu->opt_control->key_event(self->visu->opt_control, key, pressed);

    return FALSE;
}

static gboolean mouse_click_callback(GtkWidget *widget, GdkEvent *event, gpointer data) {
    VuVisuWidget *self = data;
    if (self->visu->opt_control == NULL)
        return FALSE;

    bool clicked = event->type == GDK_BUTTON_PRESS;
    self->visu->opt_control->mouse_click_event(self->visu->opt_control,
                                               (float) event->button.x, (float) event->button.y,
                                               event->button.button, clicked);

    return FALSE;
}

static gboolean mouse_move_callback(GtkWidget *widget, GdkEvent *event, gpointer data) {
    VuVisuWidget *self = data;
    if (self->visu->opt_control == NULL)
        return FALSE;

    self->visu->opt_control->mouse_move_event(self->visu->opt_control, (float) event->motion.x,
                                              (float) event->motion.y);

    return FALSE;
}

static gboolean mouse_scroll_callback(GtkWidget *widget, GdkEvent *event, gpointer data) {
    VuVisuWidget *self = data;
    if (self->visu->opt_control == NULL)
        return FALSE;

    if(event->scroll.direction != GDK_SCROLL_UP && event->scroll.direction != GDK_SCROLL_DOWN)
        return FALSE;

    bool up = event->scroll.direction == GDK_SCROLL_UP;
    self->visu->opt_control->mouse_scroll_event(self->visu->opt_control, up);

    return FALSE;
}

