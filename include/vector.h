#ifndef VECTOR_H
#define VECTOR_H

/*! @file vector.h
 *  @brief Operations with matrices.
 */

#define floatPI 3.14159265358979323846f
//                      ^
//              expected precision

/// A vector with 3 components.
typedef union {
  float raw[3];
  struct {
    float x;
    float y;
    float z;
  };
} t_vec3;

/// A 4 by 4 matrix.
typedef union {
  float raw[16];
  struct {
    float v1[4];
    float v2[4];
    float v3[4];
    float v4[4];
  };
  struct {
    float x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4;
  };
} t_mat4;

/// An object's bounding box.
typedef union {
  float raw[6];
  struct {
    float x_min, x_max, y_min, y_max, z_min, z_max;
  };
} t_bbox;

// mat4
t_mat4 mat4_create_identity(void);
t_mat4 *mat4_set_identity(t_mat4 *dest);
t_mat4 *mat4_multiply(t_mat4 *mat, t_mat4 *mat2, t_mat4 *dest);

/**
 * @brief      Apply a scaling matrix.
 *
 * @param      mat   The matrix to be scaled
 * @param      vec   The vector with scales on X, Y, Z axis.
 * @param      dest  The destination matrix
 *
 * @return     A pointer to the destination matrix.
 */
t_mat4 *mat4_scale(t_mat4 *mat, t_vec3 *vec, t_mat4 *dest);
t_mat4 *mat4_scale_float(t_mat4 *mat, float scale, t_mat4 *dest);
t_mat4 *mat4_rotate(t_mat4 *mat, float angle, t_vec3 *axis, t_mat4 *dest);
t_mat4 *mat4_rotateX(t_mat4 *mat, float angle, t_mat4 *dest);
t_mat4 *mat4_rotateY(t_mat4 *mat, float angle, t_mat4 *dest);
t_mat4 *mat4_rotateZ(t_mat4 *mat, float angle, t_mat4 *dest);
t_mat4 *mat4_translate(t_mat4 *mat, t_vec3 *vec, t_mat4 *dest);
t_mat4 *mat4_translateZ(t_mat4 *mat, float z);

t_mat4 *mat4_perspective(float fovy, float aspect, float near, float far,
                         t_mat4 *dest);
// TODO probably remove lookAt, as the camera will always point to the same spot
t_mat4 *mat4_lookAt(t_vec3 *eye, t_vec3 *center, t_vec3 *up, t_mat4 *dest);
t_mat4 *mat4_unit_box(t_bbox *bbox, t_mat4 *dest);

void print_mat4(const t_mat4 *m);

#endif
