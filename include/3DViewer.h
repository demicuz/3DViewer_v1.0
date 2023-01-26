#ifndef _3DVIEWER_H
#define _3DVIEWER_H

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"

#define GLFW_INCLUDE_NONE
#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include "vector.h"

#include <stdbool.h>

// const int WIDTH = 800, HEIGHT = 600;
// const float ASPECT = (float) WIDTH / (float) HEIGHT;

#define VSYNC true
#define WIDTH 1200
#define HEIGHT 800
#define ASPECT (float)WIDTH / (float)HEIGHT
#define CAMERA_DISTANCE 2
#define FOV 40

#define MAX_LINE_LEN 65536

// #define DEBUG_PRINT

typedef struct {
  t_bbox bbox;
  GLfloat *vertices;
  GLuint *indices;
  unsigned int vertex_count;
  unsigned int edge_count;

  t_mat4 model;
  t_mat4 view;
  t_mat4 proj;
  t_mat4 mvp;

  t_vec3 translation;
  t_vec3 rotation;
  float scale;

  GLint gl_matrix_id;
  GLint gl_line_col_id;
  bool view_was_updated;
} t_object;

typedef struct {
  t_vec3 bg_col;
  t_vec3 line_col;
  t_object *obj;
  char filepath[4096];
  char *basename;
  ImGuiIO *ioptr;
  // float fov;
  bool model_was_updated;
  bool parse_error;
} t_app;

typedef struct {
  char *basename;
  char *cursor;
  size_t lines_read;
} t_parser;

// ui
void init_ui(GLFWwindow *window, t_app *app);
void render_ui(t_app *app, t_object *obj);
void draw_ui(void);
void ui_cleanup(void);

// quite_ok_parser
bool parse_obj(const char *filepath, t_object *obj);

#endif
