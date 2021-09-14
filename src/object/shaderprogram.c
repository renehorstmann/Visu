#include <pbase/rhc/log.h>

#include "shaderprogram.h"

#define PRELOG "[Visu::Shader] "


VuShaderProgram vu_shader_program_new(const char *vertex_code, const char *fragment_code) {
    VuShaderProgram self = {0};
    self.vertex_code = vertex_code;
    self.fragment_code = fragment_code;
    self.program = 0;
    self.mv_uniform = self.mvp_uniform = -1;
    return self;
}

void vu_shader_program_ref(VuShaderProgram *self) {
    self->usages++;
}

void vu_shader_program_unref(VuShaderProgram *self) {
    if (--self->usages > 0)
        return;
    if (self->usages < 0) {
        log_wtf(PRELOG"Already killed");
        return;
    }
    glDeleteProgram(self->program);
    self->program = 0;
}

GLuint vu_shader_program_create_shader(GLint type, const char *src) {
    GLint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = malloc(log_len + 1);
        glGetShaderInfoLog(shader, log_len, NULL, buffer);
        log_warn(PRELOG"Compile failure in %s shader: %s", type == GL_VERTEX_SHADER ? "vertex" : "fragment", buffer);
        free(buffer);

        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

void vu_shader_program_compile(VuShaderProgram *self) {
    GLuint vertex = vu_shader_program_create_shader(GL_VERTEX_SHADER, self->vertex_code);
    if (vertex == 0)
        return;

    GLuint fragment = vu_shader_program_create_shader(GL_FRAGMENT_SHADER, self->fragment_code);
    if (fragment == 0) {
        glDeleteShader(vertex);
        return;
    }

    self->program = glCreateProgram();
    glAttachShader(self->program, vertex);
    glAttachShader(self->program, fragment);

    glLinkProgram(self->program);

    int status;
    glGetProgramiv(self->program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len;
        glGetProgramiv(self->program, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = malloc(log_len + 1);
        glGetProgramInfoLog(self->program, log_len, NULL, buffer);
        log_warn (PRELOG"Linking failure: %s", buffer);
        free(buffer);

        glDeleteProgram(self->program);
        self->program = 0;
    } else {
        self->mv_uniform = glGetUniformLocation(self->program, "mv");
        self->mvp_uniform = glGetUniformLocation(self->program, "mvp");
        glDetachShader(self->program, vertex);
        glDetachShader(self->program, fragment);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void vu_shader_program_use(VuShaderProgram *self, mat4 mv, mat4 mvp) {
    if (self->program == 0)
        vu_shader_program_compile(self);

    glUseProgram(self->program);
    if (self->mv_uniform != -1)
        glUniformMatrix4fv(self->mv_uniform, 1, GL_FALSE, mv.v);
    if (self->mvp_uniform != -1)
        glUniformMatrix4fv(self->mvp_uniform, 1, GL_FALSE, mvp.v);
}


void vu_light_option_uniforms_find(VuLightOptionUniforms *self, GLint program) {
    self->light_source_uniform = glGetUniformLocation(program, "light_source");
    self->light_color_uniform = glGetUniformLocation(program, "light_color");
    self->light_power_uniform = glGetUniformLocation(program, "light_power");
    self->ambient_color_uniform = glGetUniformLocation(program, "ambient_color");
}

void vu_light_option_uniforms_set(VuLightOptionUniforms self, GLint program, VuLightOptions_s options) {
    glUniform3fv(self.light_source_uniform, 1, options.light_source.v);
    glUniform3fv(self.light_color_uniform, 1, options.light_color.v);
    glUniform1f(self.light_power_uniform, options.light_power);
    glUniform3fv(self.ambient_color_uniform, 1, options.ambient_color.v);
}
