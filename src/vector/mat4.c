#include "vector.h"

#include <stddef.h>
#include <math.h>
#include <stdio.h>

t_mat4 mat4_create_identity(void) {
  t_mat4 result = {{0.0f}};

  result.x1 = 1.0f;
  result.y2 = 1.0f;
  result.z3 = 1.0f;
  result.w4 = 1.0f;

  return result;
}

t_mat4 *mat4_set_identity(t_mat4 *dest) {
  dest->raw[0] = 1;
  dest->raw[1] = 0;
  dest->raw[2] = 0;
  dest->raw[3] = 0;
  dest->raw[4] = 0;
  dest->raw[5] = 1;
  dest->raw[6] = 0;
  dest->raw[7] = 0;
  dest->raw[8] = 0;
  dest->raw[9] = 0;
  dest->raw[10] = 1;
  dest->raw[11] = 0;
  dest->raw[12] = 0;
  dest->raw[13] = 0;
  dest->raw[14] = 0;
  dest->raw[15] = 1;
  return dest;
}

t_mat4 *mat4_multiply(t_mat4 *mat, t_mat4 *mat2, t_mat4 *dest) {
  // Cache the matrix values (makes for huge speed increases!)
  float a00 = mat->raw[0], a01 = mat->raw[1], a02 = mat->raw[2],
        a03 = mat->raw[3], a10 = mat->raw[4], a11 = mat->raw[5],
        a12 = mat->raw[6], a13 = mat->raw[7], a20 = mat->raw[8],
        a21 = mat->raw[9], a22 = mat->raw[10], a23 = mat->raw[11],
        a30 = mat->raw[12], a31 = mat->raw[13], a32 = mat->raw[14],
        a33 = mat->raw[15],

        b00 = mat2->raw[0], b01 = mat2->raw[1], b02 = mat2->raw[2],
        b03 = mat2->raw[3], b10 = mat2->raw[4], b11 = mat2->raw[5],
        b12 = mat2->raw[6], b13 = mat2->raw[7], b20 = mat2->raw[8],
        b21 = mat2->raw[9], b22 = mat2->raw[10], b23 = mat2->raw[11],
        b30 = mat2->raw[12], b31 = mat2->raw[13], b32 = mat2->raw[14],
        b33 = mat2->raw[15];

  dest->raw[0] = b00 * a00 + b01 * a10 + b02 * a20 + b03 * a30;
  dest->raw[1] = b00 * a01 + b01 * a11 + b02 * a21 + b03 * a31;
  dest->raw[2] = b00 * a02 + b01 * a12 + b02 * a22 + b03 * a32;
  dest->raw[3] = b00 * a03 + b01 * a13 + b02 * a23 + b03 * a33;
  dest->raw[4] = b10 * a00 + b11 * a10 + b12 * a20 + b13 * a30;
  dest->raw[5] = b10 * a01 + b11 * a11 + b12 * a21 + b13 * a31;
  dest->raw[6] = b10 * a02 + b11 * a12 + b12 * a22 + b13 * a32;
  dest->raw[7] = b10 * a03 + b11 * a13 + b12 * a23 + b13 * a33;
  dest->raw[8] = b20 * a00 + b21 * a10 + b22 * a20 + b23 * a30;
  dest->raw[9] = b20 * a01 + b21 * a11 + b22 * a21 + b23 * a31;
  dest->raw[10] = b20 * a02 + b21 * a12 + b22 * a22 + b23 * a32;
  dest->raw[11] = b20 * a03 + b21 * a13 + b22 * a23 + b23 * a33;
  dest->raw[12] = b30 * a00 + b31 * a10 + b32 * a20 + b33 * a30;
  dest->raw[13] = b30 * a01 + b31 * a11 + b32 * a21 + b33 * a31;
  dest->raw[14] = b30 * a02 + b31 * a12 + b32 * a22 + b33 * a32;
  dest->raw[15] = b30 * a03 + b31 * a13 + b32 * a23 + b33 * a33;

  return dest;
}

t_mat4 *mat4_scale(t_mat4 *mat, t_vec3 *vec, t_mat4 *dest) {
  float x = vec->raw[0], y = vec->raw[1], z = vec->raw[2];

  if (!dest || mat == dest) {
    mat->raw[0] *= x;
    mat->raw[1] *= x;
    mat->raw[2] *= x;
    mat->raw[3] *= x;
    mat->raw[4] *= y;
    mat->raw[5] *= y;
    mat->raw[6] *= y;
    mat->raw[7] *= y;
    mat->raw[8] *= z;
    mat->raw[9] *= z;
    mat->raw[10] *= z;
    mat->raw[11] *= z;
    return mat;
  }

  dest->raw[0] = mat->raw[0] * x;
  dest->raw[1] = mat->raw[1] * x;
  dest->raw[2] = mat->raw[2] * x;
  dest->raw[3] = mat->raw[3] * x;
  dest->raw[4] = mat->raw[4] * y;
  dest->raw[5] = mat->raw[5] * y;
  dest->raw[6] = mat->raw[6] * y;
  dest->raw[7] = mat->raw[7] * y;
  dest->raw[8] = mat->raw[8] * z;
  dest->raw[9] = mat->raw[9] * z;
  dest->raw[10] = mat->raw[10] * z;
  dest->raw[11] = mat->raw[11] * z;
  dest->raw[12] = mat->raw[12];
  dest->raw[13] = mat->raw[13];
  dest->raw[14] = mat->raw[14];
  dest->raw[15] = mat->raw[15];
  return dest;
}

t_mat4 *mat4_scale_float(t_mat4 *mat, float scale, t_mat4 *dest) {
  if (!dest || mat == dest) {
    mat->raw[0] *= scale;
    mat->raw[1] *= scale;
    mat->raw[2] *= scale;
    mat->raw[3] *= scale;
    mat->raw[4] *= scale;
    mat->raw[5] *= scale;
    mat->raw[6] *= scale;
    mat->raw[7] *= scale;
    mat->raw[8] *= scale;
    mat->raw[9] *= scale;
    mat->raw[10] *= scale;
    mat->raw[11] *= scale;
    return mat;
  }

  dest->raw[0] = mat->raw[0] * scale;
  dest->raw[1] = mat->raw[1] * scale;
  dest->raw[2] = mat->raw[2] * scale;
  dest->raw[3] = mat->raw[3] * scale;
  dest->raw[4] = mat->raw[4] * scale;
  dest->raw[5] = mat->raw[5] * scale;
  dest->raw[6] = mat->raw[6] * scale;
  dest->raw[7] = mat->raw[7] * scale;
  dest->raw[8] = mat->raw[8] * scale;
  dest->raw[9] = mat->raw[9] * scale;
  dest->raw[10] = mat->raw[10] * scale;
  dest->raw[11] = mat->raw[11] * scale;
  dest->raw[12] = mat->raw[12];
  dest->raw[13] = mat->raw[13];
  dest->raw[14] = mat->raw[14];
  dest->raw[15] = mat->raw[15];
  return dest;
}

// axis is a normalized vector
t_mat4 *mat4_rotate(t_mat4 *mat, float angle, t_vec3 *axis, t_mat4 *dest) {
  float x = axis->raw[0], y = axis->raw[1], z = axis->raw[2], s, c, t, a00, a01,
        a02, a03, a10, a11, a12, a13, a20, a21, a22, a23, b00, b01, b02, b10,
        b11, b12, b20, b21, b22;

  if (x == 0 && y == 0 && z == 0) {
    return NULL;
  }

  s = sinf(angle);
  c = cosf(angle);
  t = 1 - c;

  a00 = mat->raw[0];
  a01 = mat->raw[1];
  a02 = mat->raw[2];
  a03 = mat->raw[3];
  a10 = mat->raw[4];
  a11 = mat->raw[5];
  a12 = mat->raw[6];
  a13 = mat->raw[7];
  a20 = mat->raw[8];
  a21 = mat->raw[9];
  a22 = mat->raw[10];
  a23 = mat->raw[11];

  // Construct the elements of the rotation matrix
  b00 = x * x * t + c;
  b01 = y * x * t + z * s;
  b02 = z * x * t - y * s;
  b10 = x * y * t - z * s;
  b11 = y * y * t + c;
  b12 = z * y * t + x * s;
  b20 = x * z * t + y * s;
  b21 = y * z * t - x * s;
  b22 = z * z * t + c;

  if (!dest) {
    dest = mat;
  } else if (mat != dest) {  // If the source and destination differ, copy the
                             // unchanged last row
    dest->raw[12] = mat->raw[12];
    dest->raw[13] = mat->raw[13];
    dest->raw[14] = mat->raw[14];
    dest->raw[15] = mat->raw[15];
  }

  // Perform rotation-specific matrix multiplication
  dest->raw[0] = a00 * b00 + a10 * b01 + a20 * b02;
  dest->raw[1] = a01 * b00 + a11 * b01 + a21 * b02;
  dest->raw[2] = a02 * b00 + a12 * b01 + a22 * b02;
  dest->raw[3] = a03 * b00 + a13 * b01 + a23 * b02;

  dest->raw[4] = a00 * b10 + a10 * b11 + a20 * b12;
  dest->raw[5] = a01 * b10 + a11 * b11 + a21 * b12;
  dest->raw[6] = a02 * b10 + a12 * b11 + a22 * b12;
  dest->raw[7] = a03 * b10 + a13 * b11 + a23 * b12;

  dest->raw[8] = a00 * b20 + a10 * b21 + a20 * b22;
  dest->raw[9] = a01 * b20 + a11 * b21 + a21 * b22;
  dest->raw[10] = a02 * b20 + a12 * b21 + a22 * b22;
  dest->raw[11] = a03 * b20 + a13 * b21 + a23 * b22;
  return dest;
}

t_mat4 *mat4_rotateX(t_mat4 *mat, float angle, t_mat4 *dest) {
  float s = sinf(angle), c = cosf(angle), a10 = mat->raw[4], a11 = mat->raw[5],
        a12 = mat->raw[6], a13 = mat->raw[7], a20 = mat->raw[8],
        a21 = mat->raw[9], a22 = mat->raw[10], a23 = mat->raw[11];

  if (!dest) {
    dest = mat;
  } else if (mat != dest) {  // If the source and destination differ, copy the
                             // unchanged rows
    dest->raw[0] = mat->raw[0];
    dest->raw[1] = mat->raw[1];
    dest->raw[2] = mat->raw[2];
    dest->raw[3] = mat->raw[3];

    dest->raw[12] = mat->raw[12];
    dest->raw[13] = mat->raw[13];
    dest->raw[14] = mat->raw[14];
    dest->raw[15] = mat->raw[15];
  }

  // Perform axis-specific matrix multiplication
  dest->raw[4] = a10 * c + a20 * s;
  dest->raw[5] = a11 * c + a21 * s;
  dest->raw[6] = a12 * c + a22 * s;
  dest->raw[7] = a13 * c + a23 * s;

  dest->raw[8] = a10 * -s + a20 * c;
  dest->raw[9] = a11 * -s + a21 * c;
  dest->raw[10] = a12 * -s + a22 * c;
  dest->raw[11] = a13 * -s + a23 * c;
  return dest;
}

t_mat4 *mat4_rotateY(t_mat4 *mat, float angle, t_mat4 *dest) {
  float s = sinf(angle), c = cosf(angle), a00 = mat->raw[0], a01 = mat->raw[1],
        a02 = mat->raw[2], a03 = mat->raw[3], a20 = mat->raw[8],
        a21 = mat->raw[9], a22 = mat->raw[10], a23 = mat->raw[11];

  if (!dest) {
    dest = mat;
  } else if (mat != dest) {  // If the source and destination differ, copy the
                             // unchanged rows
    dest->raw[4] = mat->raw[4];
    dest->raw[5] = mat->raw[5];
    dest->raw[6] = mat->raw[6];
    dest->raw[7] = mat->raw[7];

    dest->raw[12] = mat->raw[12];
    dest->raw[13] = mat->raw[13];
    dest->raw[14] = mat->raw[14];
    dest->raw[15] = mat->raw[15];
  }

  // Perform axis-specific matrix multiplication
  dest->raw[0] = a00 * c + a20 * -s;
  dest->raw[1] = a01 * c + a21 * -s;
  dest->raw[2] = a02 * c + a22 * -s;
  dest->raw[3] = a03 * c + a23 * -s;

  dest->raw[8] = a00 * s + a20 * c;
  dest->raw[9] = a01 * s + a21 * c;
  dest->raw[10] = a02 * s + a22 * c;
  dest->raw[11] = a03 * s + a23 * c;
  return dest;
}

t_mat4 *mat4_rotateZ(t_mat4 *mat, float angle, t_mat4 *dest) {
  float s = sinf(angle), c = cosf(angle), a00 = mat->raw[0], a01 = mat->raw[1],
        a02 = mat->raw[2], a03 = mat->raw[3], a10 = mat->raw[4],
        a11 = mat->raw[5], a12 = mat->raw[6], a13 = mat->raw[7];

  if (!dest) {
    dest = mat;
  } else if (mat != dest) {  // If the source and destination differ, copy the
                             // unchanged last row
    dest->raw[8] = mat->raw[8];
    dest->raw[9] = mat->raw[9];
    dest->raw[10] = mat->raw[10];
    dest->raw[11] = mat->raw[11];

    dest->raw[12] = mat->raw[12];
    dest->raw[13] = mat->raw[13];
    dest->raw[14] = mat->raw[14];
    dest->raw[15] = mat->raw[15];
  }

  // Perform axis-specific matrix multiplication
  dest->raw[0] = a00 * c + a10 * s;
  dest->raw[1] = a01 * c + a11 * s;
  dest->raw[2] = a02 * c + a12 * s;
  dest->raw[3] = a03 * c + a13 * s;

  dest->raw[4] = a00 * -s + a10 * c;
  dest->raw[5] = a01 * -s + a11 * c;
  dest->raw[6] = a02 * -s + a12 * c;
  dest->raw[7] = a03 * -s + a13 * c;

  return dest;
}

t_mat4 *mat4_translate(t_mat4 *mat, t_vec3 *vec, t_mat4 *dest) {
  float *m = mat->raw;
  float *v = vec->raw;
  float *d = dest->raw;
  float x = v[0], y = v[1], z = v[2], a00, a01, a02, a03, a10, a11, a12, a13,
        a20, a21, a22, a23;

  if (!dest || mat == dest) {
    m[12] = m[0] * x + m[4] * y + m[8] * z + m[12];
    m[13] = m[1] * x + m[5] * y + m[9] * z + m[13];
    m[14] = m[2] * x + m[6] * y + m[10] * z + m[14];
    m[15] = m[3] * x + m[7] * y + m[11] * z + m[15];
    return mat;
  }

  a00 = m[0];
  a01 = m[1];
  a02 = m[2];
  a03 = m[3];
  a10 = m[4];
  a11 = m[5];
  a12 = m[6];
  a13 = m[7];
  a20 = m[8];
  a21 = m[9];
  a22 = m[10];
  a23 = m[11];

  d[0] = a00;
  d[1] = a01;
  d[2] = a02;
  d[3] = a03;
  d[4] = a10;
  d[5] = a11;
  d[6] = a12;
  d[7] = a13;
  d[8] = a20;
  d[9] = a21;
  d[10] = a22;
  d[11] = a23;

  d[12] = a00 * x + a10 * y + a20 * z + mat->raw[12];
  d[13] = a01 * x + a11 * y + a21 * z + mat->raw[13];
  d[14] = a02 * x + a12 * y + a22 * z + mat->raw[14];
  d[15] = a03 * x + a13 * y + a23 * z + mat->raw[15];
  return dest;
}

t_mat4 *mat4_translateZ(t_mat4 *mat, float z) {
  float *m = mat->raw;

  m[12] = m[8] * z + m[12];
  m[13] = m[9] * z + m[13];
  m[14] = m[10] * z + m[14];
  m[15] = m[11] * z + m[15];
  return mat;
}

t_mat4 *mat4_frustum(float left, float right, float bottom, float top,
                     float near, float far, t_mat4 *dest) {
  float rl = (right - left);
  float tb = (top - bottom);
  float fn = (far - near);
  dest->raw[0] = (near * 2) / rl;
  dest->raw[1] = 0;
  dest->raw[2] = 0;
  dest->raw[3] = 0;
  dest->raw[4] = 0;
  dest->raw[5] = (near * 2) / tb;
  dest->raw[6] = 0;
  dest->raw[7] = 0;
  dest->raw[8] = (right + left) / rl;
  dest->raw[9] = (top + bottom) / tb;
  dest->raw[10] = -(far + near) / fn;
  dest->raw[11] = -1;
  dest->raw[12] = 0;
  dest->raw[13] = 0;
  dest->raw[14] = -(far * near * 2) / fn;
  dest->raw[15] = 0;
  return dest;
}

t_mat4 *mat4_perspective(float fovy, float aspect, float near, float far,
                         t_mat4 *dest) {
  float top = near * tanf(fovy * floatPI / 360.0f);
  float right = top * aspect;
  return mat4_frustum(-right, right, -top, top, near, far, dest);
}

t_mat4 *mat4_unit_box(t_bbox *bbox, t_mat4 *dest) {
  float x_span = bbox->x_max - bbox->x_min;
  float y_span = bbox->y_max - bbox->y_min;
  float z_span = bbox->z_max - bbox->z_min;
  float max_span = fmaxf(fmaxf(x_span, y_span), z_span);

  t_vec3 translation = {{-(bbox->x_min + bbox->x_max) / 2,
                         -(bbox->y_min + bbox->y_max) / 2,
                         -(bbox->z_min + bbox->z_max) / 2}};

  mat4_scale_float(dest, 1.0f / max_span, NULL);
  mat4_translate(dest, &translation, NULL);

  return dest;
}

void print_mat4(const t_mat4 *m) {
  printf(
      "%.2f %.2f %.2f %.2f\n"
      "%.2f %.2f %.2f %.2f\n"
      "%.2f %.2f %.2f %.2f\n"
      "%.2f %.2f %.2f %.2f\n",
      m->raw[0], m->raw[4], m->raw[8], m->raw[12], m->raw[1], m->raw[5],
      m->raw[9], m->raw[13], m->raw[2], m->raw[6], m->raw[10], m->raw[14],
      m->raw[3], m->raw[7], m->raw[11], m->raw[15]);
}
