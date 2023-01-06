#include <glad/gl.h>

#include <stdio.h>
#include <stdbool.h>

// Stolen from https://github.com/tsoding/opengl-template

static const char *shader_type_as_cstr(GLuint shader) {
  switch (shader) {
  case GL_VERTEX_SHADER:
    return "GL_VERTEX_SHADER";
  case GL_FRAGMENT_SHADER:
    return "GL_FRAGMENT_SHADER";
  default:
    return "(Unknown)";
  }
}

static bool compile_shader_source(const GLchar *source, GLenum shader_type,
                                  GLuint *shader) {
  *shader = glCreateShader(shader_type);
  glShaderSource(*shader, 1, &source, NULL);
  glCompileShader(*shader);

  GLint compiled = 0;
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled) {
    GLchar message[1024];
    GLsizei message_size = 0;
    glGetShaderInfoLog(*shader, sizeof(message), &message_size, message);
    (void)fprintf(stderr, "ERROR: could not compile %s\n",
                  shader_type_as_cstr(shader_type));
    (void)fprintf(stderr, "%.*s\n", message_size, message);
    return false;
  }

  return true;
}

static bool link_program(GLuint vert_shader, GLuint frag_shader,
                         GLuint *program) {
  *program = glCreateProgram();

  glAttachShader(*program, vert_shader);
  glAttachShader(*program, frag_shader);
  glLinkProgram(*program);

  GLint linked = 0;
  glGetProgramiv(*program, GL_LINK_STATUS, &linked);
  if (!linked) {
    GLsizei message_size = 0;
    GLchar message[1024];

    glGetProgramInfoLog(*program, sizeof(message), &message_size, message);
    (void)fprintf(stderr, "Program Linking: %.*s\n", message_size, message);
  }

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  return program;
}

bool create_line_shader_program(GLuint *program) {
  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "uniform mat4 MVP;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = MVP * vec4(aPos, 1.0);\n"
      "}\0"; // TODO aren't they null-terminated by default?

  const char *fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\0"; // TODO aren't they null-terminated by default?

  GLuint vertexShader;
  if (!compile_shader_source(vertexShaderSource, GL_VERTEX_SHADER,
                             &vertexShader)) {
    return false;
  }

  GLuint fragmentShader;
  if (!compile_shader_source(fragmentShaderSource, GL_FRAGMENT_SHADER,
                             &fragmentShader)) {
    glDeleteShader(vertexShader);
    return false;
  }

  return link_program(vertexShader, fragmentShader, program);
}
