#ifndef GL_WRAPPER_H
#define GL_WRAPPER_H

#define GLFW_INCLUDE_NONE
#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include <stdbool.h>

GLFWwindow *get_glfw_window(void);
bool create_line_shader_program(GLuint *program);

#endif
