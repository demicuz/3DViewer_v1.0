#ifndef _3DVIEWER_H
#define _3DVIEWER_H

#include "glad/gl.h"
#include "vector.h"

#include <stdbool.h>

// const int WIDTH = 800, HEIGHT = 600;
// const float ASPECT = (float) WIDTH / (float) HEIGHT;

#define VSYNC false
#define WIDTH 800
#define HEIGHT 600
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
} t_object;

#endif
