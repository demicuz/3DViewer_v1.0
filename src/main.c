#include "3DViewer.h"
#include "gl-wrapper.h"
#include "vector.h"

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

bool init_obj(t_object *obj) {
  *obj = (t_object){0};
  obj->scale = 1;

  mat4_set_identity(&obj->model);

  obj->bbox = (t_bbox){.x_min = -1,
                       .x_max = 1,
                       .y_min = -1,
                       .y_max = 1,
                       .z_min = -1,
                       .z_max = 1};
  mat4_unit_box(&obj->bbox, &obj->model);

  mat4_set_identity(&obj->view);
  mat4_translateZ(&obj->view, -CAMERA_DISTANCE);

  mat4_perspective(FOV, ASPECT, 0.1f, 1e5f, &obj->proj);

  return true;
}

void update_view_mat(t_object *obj) {
  mat4_set_identity(&obj->view);

  mat4_translateZ(&obj->view, -CAMERA_DISTANCE);
  mat4_translate(&obj->view, &obj->translation, NULL);

  mat4_rotateX(&obj->view, obj->rotation.x, NULL);
  mat4_rotateY(&obj->view, obj->rotation.y, NULL);
  mat4_rotateZ(&obj->view, obj->rotation.z, NULL);

  mat4_scale_float(&obj->view, obj->scale, NULL);
}

int main(void) {
  GLFWwindow *window = get_glfw_window();
  init_ui(window);

  GLuint shaderProgram;
  if (!create_line_shader_program(&shaderProgram)) {
    glfwTerminate();
    exit(1);
  }

  // clang-format off
  GLfloat cube_vertices[] = {
    // front face
     1,  1, -1, // 0 top right
     1, -1, -1, // 1 bottom right
    -1, -1, -1, // 2 bottom left
    -1,  1, -1, // 3 top left
    // back face
     1,  1,  1, // 4 top right
     1, -1,  1, // 5 bottom right
    -1, -1,  1, // 6 bottom left
    -1,  1,  1  // 7 top left
  };
  // clang-format on

  // TODO a data structure for non-repetitive lines? Now we're drawing each line
  // at least two times. There could be more, depending on the model.
  GLuint indices[] = {
      0, 1, 1, 2, 2, 3, 3, 0, // front face
      4, 5, 5, 6, 6, 7, 7, 4, // back face
      2, 3, 3, 7, 7, 6, 6, 2, // left face
      0, 1, 1, 5, 5, 4, 4, 0, // right face
      0, 4, 4, 7, 7, 3, 3, 0, // top face
      1, 5, 5, 6, 6, 2, 2, 1, // bottom face
  };

  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // TODO where do I put these?
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(posAttrib);

  glUseProgram(shaderProgram);

  t_object obj;
  init_obj(&obj);
  obj.gl_matrix_id = glGetUniformLocation(shaderProgram, "MVP");

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(3.0f);

  float t = 0;
  t_app app = {0};
  while (!glfwWindowShouldClose(window)) {
    // TODO probably should update OpenGL state after GUI messes around with it
    // glUseProgram(shaderProgram);
    // glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // t = (float) glfwGetTime();
    // obj.translation.x = sinf(t) / 2;
    // obj.rotation.y = t;
    // obj.rotation.z = 2 * t;
    // obj.scale = 0.8f + cosf(2.71828f * t + 0.3f) * 0.2f;
    update_view_mat(&obj);
    // TODO update_proj_mat(&obj) in case we're gonna change FOV
    mat4_multiply(&obj.proj, &obj.view, &obj.mvp);
    mat4_multiply(&obj.mvp, &obj.model, &obj.mvp);
    glUniformMatrix4fv(obj.gl_matrix_id, 1, GL_FALSE, obj.mvp.raw);

    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_LINES, sizeof indices / sizeof(GLuint), GL_UNSIGNED_INT,
                   0);
    // glMultiDrawElements(GL_LINE_LOOP, cube_counts, GL_UNSIGNED_INT, (const
    // void **) cube_indices, sizeof cube_counts / sizeof (GLsizei));
    render_ui(&app, &obj);

    // Swap the screen buffers
    glfwSwapBuffers(window);

    // glfwPollEvents(); // Update 60 times per second
    glfwWaitEvents(); // Update only on key press, mouse move, etc.
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  ui_cleanup();
  // Terminates GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
  exit(0);
}
