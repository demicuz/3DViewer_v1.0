#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include <3DViewer.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // TODO: Also update projection matrix, as it depends on the aspect ratio.
  // You can't do that without globals, as GLFW lacks custom window data.
  glViewport(0, 0, width, height);
}

GLFWwindow *get_glfw_window(void) {
  if (glfwInit() == GLFW_FALSE) {
    (void)fprintf(stderr, "Failed to initialize GLFW\n");
    exit(1);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__ // TODO do I even need this?
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "3DViewer", NULL, NULL);
  if (window == NULL) {
    (void)fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate(); // TODO maybe not needed if completely shutting down
    exit(1);
  }
  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    (void)fprintf(stderr, "Failed to initialize OpenGL context\n");
    glfwTerminate(); // TODO maybe not needed if completely shutting down
    exit(1);
  }
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Successfully loaded OpenGL
  printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version),
         GLAD_VERSION_MINOR(version));

  return window;
}
