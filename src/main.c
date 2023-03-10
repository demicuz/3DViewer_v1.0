#include "3DViewer.h"
#include "gl-wrapper.h"
#include "vector.h"
#include "dynamic-array.h"

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool init_obj(t_object *obj) {
  obj->scale = 1;
  obj->view_was_updated = true;

  mat4_set_identity(&obj->model);
  mat4_unit_box(&obj->bbox, &obj->model);

  mat4_set_identity(&obj->view);
  mat4_translateZ(&obj->view, -CAMERA_DISTANCE);

  mat4_perspective(FOV, ASPECT, 0.1f, 1e5f, &obj->proj);

  return true;
}

void update_view_mat(t_object *obj) {
  mat4_set_identity(&obj->view);

  mat4_scale_float(&obj->view, obj->scale, NULL);

  mat4_translateZ(&obj->view, -CAMERA_DISTANCE / obj->scale);
  mat4_translate(&obj->view, &obj->translation, NULL);

  mat4_rotateX(&obj->view, obj->rotation.x, NULL);
  mat4_rotateY(&obj->view, obj->rotation.y, NULL);
  mat4_rotateZ(&obj->view, obj->rotation.z, NULL);

#ifdef DEBUG_PRINT
  puts("View mat:");
  print_mat4(&obj->view);
#endif
}

void update_mvp(t_object *obj) {
  mat4_multiply(&obj->proj, &obj->view, &obj->mvp);
  mat4_multiply(&obj->mvp, &obj->model, &obj->mvp);
  glUniformMatrix4fv(obj->gl_matrix_id, 1, GL_FALSE, obj->mvp.raw);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  (void)scancode;
  (void)mode;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

// GLFW lacks custom window data in callbacks, so we have to use globals.
static float aspect = ASPECT;
static bool window_was_resized = false;
void resize_callback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
  aspect = (float)width / (float)height;
  window_was_resized = true;
}

int main(void) {
  t_app app = {0};
  t_object obj = {0};

  app.obj = &obj;
  init_obj(&obj);
  app.bg_col = (t_vec3){{0.2f, 0.3f, 0.3f}};
  app.line_col = (t_vec3){{1, 0.5f, 0.2f}};

  GLFWwindow *window = get_glfw_window();
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, resize_callback);

  init_ui(window, &app);

  GLuint shaderProgram;
  if (!create_line_shader_program(&shaderProgram)) {
    glfwTerminate();
    exit(1);
  }

  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(posAttrib);

  glUseProgram(shaderProgram);

  obj.gl_matrix_id = glGetUniformLocation(shaderProgram, "MVP");
  obj.gl_line_col_id = glGetUniformLocation(shaderProgram, "line_col");

  // glLineWidth(3.0f);

  while (!glfwWindowShouldClose(window)) {
    render_ui(&app, &obj);
    // If obj state was changed with UI, update MVP
    if (obj.view_was_updated) {
      update_view_mat(&obj);
      update_mvp(&obj);
      obj.view_was_updated = false;
    } else if (window_was_resized) {
      mat4_perspective(FOV, aspect, 0.1f, 1e5f, &obj.proj);
      update_mvp(&obj);
      window_was_resized = false;
    } else if (app.model_was_updated) {
      printf("vertices: %d\n", obj.vertex_count);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, array_size(obj.vertices) * sizeof(GLfloat),
                   obj.vertices, GL_STATIC_DRAW);

      printf("indices: %d\n", array_size(obj.indices) / 2);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   array_size(obj.indices) * sizeof(GLuint), obj.indices,
                   GL_STATIC_DRAW);

#ifdef DEBUG_PRINT
      printf("bounding box:\n%.2f %.2f\n%.2f %.2f\n%.2f %.2f\n", obj.bbox.x_min,
             obj.bbox.x_max, obj.bbox.y_min, obj.bbox.y_max, obj.bbox.z_min,
             obj.bbox.z_max);
#endif

      mat4_set_identity(&obj.model);
      mat4_unit_box(&obj.bbox, &obj.model);
      update_mvp(&obj);
      app.model_was_updated = false;
    }
    // TODO update_proj_mat(&obj) in case we're gonna change FOV

    // Draw
    glClearColor(app.bg_col.x, app.bg_col.y, app.bg_col.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform3fv(obj.gl_line_col_id, 1, app.line_col.raw);
    glDrawElements(GL_LINES, array_size(obj.indices), GL_UNSIGNED_INT, 0);

    draw_ui();

    // Swap the screen buffers
    glfwSwapBuffers(window);

    // glfwPollEvents(); // Update 60 times per second
    glfwWaitEvents();  // Update only on key press, mouse move, etc.
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  ui_cleanup();
  array_clean(obj.vertices);
  array_clean(obj.indices);

  glfwTerminate();
  exit(0);
}
