#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include <3DViewer.h>
#include <gl-wrapper.h>
#include <vector.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // TODO also update projection matrix, as it depends on the aspect ratio
  glViewport(0, 0, width, height);
}

int main(void) {
  glfwInit();
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

  // Successfully loaded OpenGL
  printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version),
         GLAD_VERSION_MINOR(version));

  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  GLuint shaderProgram;
  create_line_shader_program(&shaderProgram);

  // clang-format off
  GLfloat cube_vertices[] = {
    // front face
     0.3f,  0.3f, -0.3f, // 0 top right
     0.3f, -0.3f, -0.3f, // 1 bottom right
    -0.3f, -0.3f, -0.3f, // 2 bottom left
    -0.3f,  0.3f, -0.3f, // 3 top left
    // back face
     0.3f,  0.3f,  0.3f, // 4 top right
     0.3f, -0.3f,  0.3f, // 5 bottom right
    -0.3f, -0.3f,  0.3f, // 6 bottom left
    -0.3f,  0.3f,  0.3f  // 7 top left
  };
  // clang-format on

  // TODO a datastructure for non-repetitive lines
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

  t_mat4 model = mat4_create_identity();

  t_vec3 translation = vec3(0, 0, -1);
  mat4_translate(&model, &translation, NULL);

  t_vec3 y_axis = vec3(0, 1, 0);
  mat4_rotate(&model, 0.2f, &y_axis, NULL);

  t_vec3 scale = vec3(0.5f, 0.5f, 0.5f);
  mat4_scale(&model, &scale, NULL);

  t_mat4 proj = mat4_create_identity();
  mat4_perspective(40.0f, ASPECT, 0.1f, 1e5f, &proj);

  t_mat4 mvp;
  mat4_multiply(&proj, &model, &mvp);

  GLint matrixID = glGetUniformLocation(shaderProgram, "MVP");
  glUniformMatrix4fv(matrixID, 1, GL_FALSE, mvp.raw);

  // TODO should I also do this in the render loop?
  // glBindVertexArray(VAO);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(3.0f);

  while (!glfwWindowShouldClose(window)) {
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render
    // glUseProgram(shaderProgram);
    // glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    mat4_rotate(&mvp, 0.02f, &y_axis, NULL);
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, mvp.raw);

    glDrawElements(GL_LINES, sizeof indices / sizeof(GLuint), GL_UNSIGNED_INT,
                   0);
    // glMultiDrawElements(GL_LINE_LOOP, cube_counts, GL_UNSIGNED_INT, (const
    // void **) cube_indices, sizeof cube_counts / sizeof (GLsizei));

    // Swap the screen buffers
    glfwSwapBuffers(window);

    // Check if any events have been activated (key pressed, mouse moved etc.)
    // and call corresponding response functions
    glfwPollEvents();
    // glfwWaitEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  // Terminates GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
  exit(0);
}
