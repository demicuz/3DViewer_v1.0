#include "3DViewer.h"

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

GLFWwindow *get_glfw_window(void) {
  if (glfwInit() == GLFW_FALSE) {
    (void)fprintf(stderr, "Failed to initialize GLFW\n");
    exit(1);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "3DViewer", NULL, NULL);
  if (window == NULL) {
    (void)fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    exit(1);
  }
  glfwMakeContextCurrent(window);
  if (VSYNC) {
    glfwSwapInterval(1);
  }

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    (void)fprintf(stderr, "Failed to initialize OpenGL context\n");
    glfwTerminate();
    exit(1);
  }

  // Successfully loaded OpenGL
  printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version),
         GLAD_VERSION_MINOR(version));

  return window;
}
