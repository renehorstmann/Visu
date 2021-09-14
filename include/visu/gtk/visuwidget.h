#ifndef VISU_GTK_VISUWIDGET_H
#define VISU_GTK_VISUWIDGET_H


struct _GtkWidget;
struct _GtkWindow;

struct VuVisu;

/**
 * NOT A GTK CLASS!
 * Simple struct class containing the actual GtkGLArea and a pointer to Visu.
 */
typedef struct {
    struct _GtkWidget *glarea;
    struct VuVisu *visu;
} VuVisuWidget;

/** Creates the GlArea Widget and combines it with Visu */
VuVisuWidget *vu_visu_widget_new(struct VuVisu *visu);

void vu_visu_widget_kill(VuVisuWidget **self_ptr);

/** Turn on camera opt_control (sets the needed callbacks) */
void vu_visu_widget_set_control_callbacks(VuVisuWidget *self, struct _GtkWindow *window);


#endif //VISU_GTK_VISUWIDGET_H
