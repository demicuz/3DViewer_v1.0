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

typedef struct {
  t_bbox bbox;

  t_mat4 model;
  t_mat4 view;
  t_mat4 proj;
  t_mat4 mvp;

  t_vec3 translation;
  t_vec3 rotation;
  float scale;

  GLint gl_matrix_id;
  bool view_was_updated;
} t_object;

// TODO may be obsolete
typedef struct {
  t_object *obj;
  ImGuiIO *ioptr;
} t_app;

// ui
void init_ui(GLFWwindow *window, t_app *app);
void render_ui(t_app *app, t_object *obj);
void draw_ui(void);
void ui_cleanup(void);

#endif
