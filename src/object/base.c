#include <stdio.h>
#include <string.h>

#include <epoxy/gl.h>

#include <pbase/rhc/allocator.h>
#include <pbase/mathc/float.h>

#include "visu/visubase.h"
#include "visu/object/base.h"
#include "objectbase.h"


// private function, called by visu via VisuResource
static void resource_unload(VuVisuResource *self) {
    VuRenderObject *object = self->impl;
    assert(object->impl && "unload after kill?");

    ObjectBase *base = object->impl;
    if (base->loaded) {
        base->unload(object);
        vu_shader_program_unref(base->program);  // if unloaded, decrement the ShaderProgram counter
        base->loaded = false;
    }
}

// private function, called by visu via VisuResource
static void resource_kill(VuVisuResource *self) {
    VuRenderObject *object = self->impl;

    assert(object->impl && "multiple kill calls on a RenderObject?");

    if (object->on_kill_callback)
        object->on_kill_callback(object, object->on_kill_callback_user_data);

    ObjectBase *base = object->impl;
    base->kill(object);
    free(object);
}

// protected function (declared in objectbase.h)
void vu_render_object_init(VuRenderObject *self, VuVisu *visu) {
    self->pose = mat4_eye();
    self->used_pose = &self->pose;
    self->update = true;

    self->visu_resource = (VuVisuResource) {self, resource_unload, resource_kill};
    vu_visu_add_resource(visu, self->visu_resource);
}

void vu_render_object_kill(VuRenderObject **self_ptr) {
    VuRenderObject *self = *self_ptr;
    if(self) {
        ObjectBase *base = self->impl;
        vu_visu_kill_resource(base->visu, self->visu_resource);
    }
    *self_ptr = NULL;
}

void vu_render_object_render(VuRenderObject *self) {
    ObjectBase *base = self->impl;

    mat4 mv = mat4_mul_mat(base->visu->view, *self->used_pose);
    mat4 mvp = mat4_mul_mat(base->visu->projection, mv);

    vu_shader_program_use(base->program, mv, mvp);

    if (self->update) {
        self->update = false;
        bool was_loaded = base->loaded;
        base->loaded = base->update(self);

        if (!was_loaded && base->loaded)
            vu_shader_program_ref(base->program);  // if loaded, increment the ShaderProgram counter
    }

    if (base->loaded)
        base->render(self);

    // reset settings
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void *vu_render_object_property(VuRenderObject *self, const char *name) {
    ObjectBase *base = self->impl;

    for (int i = 0; i < base->properties_size; i++) {
        if (strcmp(name, base->properties[i].name) == 0)
            return base->properties[i].data;
    }
    return NULL;
}

const char *vu_render_object_property_type(const VuRenderObject *self, const char *name) {
    ObjectBase *base = self->impl;

    for (int i = 0; i < base->properties_size; i++) {
        if (strcmp(name, base->properties[i].name) == 0)
            return base->properties[i].type;
    }
    return NULL;
}

const char **vu_render_object_property_names_as_list_on_heap(const VuRenderObject *self) {
    ObjectBase *base = self->impl;

    const char **list = p_rhc_calloc(sizeof(const char *) * (base->properties_size + 1));
    for (int i = 0; i < base->properties_size; i++)
        list[i] = base->properties[i].name;
    return list;
}

