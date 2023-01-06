#ifndef _3DVIEWER_H
#define _3DVIEWER_H

#include "glad/gl.h"
#include "vector.h"

// const int WIDTH = 800, HEIGHT = 600;
// const float ASPECT = (float) WIDTH / (float) HEIGHT;

#define WIDTH 800
#define HEIGHT 600
#define ASPECT (float)WIDTH / (float)HEIGHT

typedef struct {
  float x_min;
  float x_max;
  float y_min;
  float y_max;
  float z_min;
  float z_max;
} t_bbox;

typedef struct {
  t_bbox bbox;
  t_mat4 model;
  t_mat4 view;
  t_mat4 proj;
  GLint gl_matrix_id;
} t_object;

#endif
