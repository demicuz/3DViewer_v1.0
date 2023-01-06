#ifndef VECTOR_H
#define VECTOR_H

typedef union {
  float raw[3];
  struct {
    float x;
    float y;
    float z;
  };
} t_vec3;

typedef union {
  float raw[16];
  struct {
    float v1[4];
    float v2[4];
    float v3[4];
    float v4[4];
  };
  struct {
    float x1;
    float y1;
    float z1;
    float w1;
    float x2;
    float y2;
    float z2;
    float w2;
    float x3;
    float y3;
    float z3;
    float w3;
    float x4;
    float y4;
    float z4;
    float w4;
  };
} t_mat4;

// vec3
t_vec3 vec3(float x, float y, float z);

// mat4
t_mat4 mat4_create_identity(void);
t_mat4 *mat4_multiply(t_mat4 *mat, t_mat4 *mat2, t_mat4 *dest);

t_mat4 *mat4_scale(t_mat4 *mat, t_vec3 *vec, t_mat4 *dest);
t_mat4 *mat4_rotate(t_mat4 *mat, float angle, t_vec3 *axis, t_mat4 *dest);
t_mat4 *mat4_translate(t_mat4 *mat, t_vec3 *vec, t_mat4 *dest);

t_mat4 *mat4_perspective(float fovy, float aspect, float near, float far,
                         t_mat4 *dest);
// TODO probably remove lookAt, as the camera will always point to the same spot
t_mat4 *mat4_lookAt(t_vec3 *eye, t_vec3 *center, t_vec3 *up, t_mat4 *dest);

// t_vec3   vec3(float x, float y, float z);
// t_vec3   v_add(t_vec3 a, t_vec3 b);
// t_vec3   v_sub(t_vec3 a, t_vec3 b);
// t_vec3   v_mul(t_vec3 v, float factor);
// t_vec3   v_mulv(t_vec3 v, t_vec3 factor);
// t_vec3   v_divv(t_vec3 v, t_vec3 factor);
// t_vec3   v_min(t_vec3 a, float min);
// float    dot(t_vec3 a, t_vec3 b);
// float    length(t_vec3 a);
// t_vec3   normalize(t_vec3 a);
// t_vec3   v_lerp(t_vec3 *a, t_vec3 *b, float t);
// t_vec3   at(const t_ray3 *r, float t);
// t_vec3   reflect(t_vec3 incident, t_vec3 normal);

#endif
