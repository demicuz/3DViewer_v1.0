#ifndef GL_WRAPPER_H
#define GL_WRAPPER_H

#include "glad/gl.h"
#include <stdbool.h>

bool compile_shader_source(const GLchar *source, GLenum shader_type, GLuint *shader);
bool link_program(GLuint vert_shader, GLuint frag_shader, GLuint *program);

#endif
