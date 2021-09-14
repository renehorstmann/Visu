#ifndef VISU_OBJECTS_SHADER_H
#define VISU_OBJECTS_SHADER_H


#include <epoxy/gl.h>
#include "stdlib.h"

#include "visu/object/mesh.h"   // for VuLightOptions_s

// This header is not public...

typedef struct VuShaderProgram {
    const char *vertex_code;
    const char *fragment_code;
    GLuint program;
    GLint mv_uniform;
    GLint mvp_uniform;
    int usages;
} VuShaderProgram;

typedef struct VuLightOptionUniforms {
    GLuint light_source_uniform;
    GLuint light_color_uniform;
    GLuint light_power_uniform;
    GLuint ambient_color_uniform;
} VuLightOptionUniforms;

VuShaderProgram vu_shader_program_new(const char *vertex_code, const char *fragment_code);

void vu_shader_program_ref(VuShaderProgram *self);

void vu_shader_program_unref(VuShaderProgram *self);

GLuint vu_shader_program_create_shader(GLint type, const char *src);

void vu_shader_program_compile(VuShaderProgram *self);

void vu_shader_program_use(VuShaderProgram *self, mat4 mv, mat4 mvp);

void vu_light_option_uniforms_find(VuLightOptionUniforms *self, GLint program);

void vu_light_option_uniforms_set(VuLightOptionUniforms self, GLint program, VuLightOptions_s options);


#endif //VISU_OBJECTS_SHADER_H
