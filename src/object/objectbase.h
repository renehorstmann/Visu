#ifndef VISU_OBJECTBASE_H
#define VISU_OBJECTBASE_H


#include <stdlib.h>
#include <string.h>

#include <pbase/mathc/float.h>
#include <pbase/rhc/alloc.h>

#include "visu/visubase.h"
#include "visu/object/base.h"
#include "shaderprogram.h"

// This header is not public...

#define VU_MAX_PROPERTIES 32


// forward declaration for childs...
void vu_render_object_init(VuRenderObject *self, VuVisu *visu);


typedef struct Property {
    const char *name;
    const char *type;
    void *data;
} Property;

typedef struct ObjectBase {
    VuVisu *visu;
    VuShaderProgram *program;

    Property properties[VU_MAX_PROPERTIES];
    int properties_size;

    bool loaded;

    void (*unload)(VuRenderObject *object);

    void (*kill)(VuRenderObject *object);

    bool (*update)(VuRenderObject *object);

    void (*render)(VuRenderObject *object);

} ObjectBase;


static void object_base_add_property(ObjectBase *self, const char *name, const char *type, void *data) {
    assert(self->properties_size < VU_MAX_PROPERTIES);
    self->properties[self->properties_size++] = (Property) {name, type, data};
}

static void object_base_add_light_option_properties_and_init(ObjectBase *self, VuLightOptions_s *options) {
    object_base_add_property(self, "light_source", "vec3", options->light_source.v);
    object_base_add_property(self, "light_color", "vec3", options->light_color.v);
    object_base_add_property(self, "light_power", "float", &options->light_power);
    object_base_add_property(self, "ambient_color", "vec3", options->ambient_color.v);

    options->light_source = VU_LIGHT_SOURCE_INIT;
    options->light_color = VU_LIGHT_COLOR_INIT;
    options->light_power = VU_LIGHT_POWER_INIT;
    options->ambient_color = VU_AMBIENT_COLOR_INIT;
}

static void object_base_init(ObjectBase *self,
                             VuVisu *visu,
                             const char *name,
                             VuShaderProgram *program,
                             void (*unload)(VuRenderObject *object),
                             void (*kill)(VuRenderObject *object),
                             bool (*update)(VuRenderObject *object),
                             void (*render)(VuRenderObject *object)) {
    self->visu = visu;
    self->program = program;
    self->unload = unload;
    self->kill = kill;
    self->update = update;
    self->render = render;

    self->properties_size = 0;
    object_base_add_property(self, "name", "const char *", (void *) name);

    self->loaded = false;
}


#endif //VISU_OBJECTBASE_H
