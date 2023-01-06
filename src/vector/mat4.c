#include <stddef.h>
#include <math.h>

#include <vector.h>

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
  } else if (mat != dest) { // If the source and destination differ, copy the
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
  float top = near * tanf(fovy * 3.14159265358979323846f / 360.0f);
  float right = top * aspect;
  return mat4_frustum(-right, right, -top, top, near, far, dest);
}

t_mat4 *mat4_lookAt(t_vec3 *eye, t_vec3 *center, t_vec3 *up, t_mat4 *dest) {
  // if (!dest) { dest = mat4_create(NULL); }

  float x0, x1, x2, y0, y1, y2, z0, z1, z2, len;
  float eyex = eye->raw[0];
  float eyey = eye->raw[1];
  float eyez = eye->raw[2];
  float upx = up->raw[0];
  float upy = up->raw[1];
  float upz = up->raw[2];
  float centerx = center->raw[0];
  float centery = center->raw[1];
  float centerz = center->raw[2];

  if (eyex == centerx && eyey == centery && eyez == centerz) {
    return mat4_set_identity(dest);
  }

  // vec3.direction(eye, center, z);
  z0 = eyex - centerx;
  z1 = eyey - centery;
  z2 = eyez - centerz;

  // normalize (no check needed for 0 because of early return)
  len = 1 / sqrtf(z0 * z0 + z1 * z1 + z2 * z2);
  z0 *= len;
  z1 *= len;
  z2 *= len;

  // vec3.normalize(vec3.cross(up, z, x));
  x0 = upy * z2 - upz * z1;
  x1 = upz * z0 - upx * z2;
  x2 = upx * z1 - upy * z0;
  len = sqrtf(x0 * x0 + x1 * x1 + x2 * x2);
  if (!len) {
    x0 = 0;
    x1 = 0;
    x2 = 0;
  } else {
    len = 1 / len;
    x0 *= len;
    x1 *= len;
    x2 *= len;
  }

  // vec3.normalize(vec3.cross(z, x, y));
  y0 = z1 * x2 - z2 * x1;
  y1 = z2 * x0 - z0 * x2;
  y2 = z0 * x1 - z1 * x0;

  len = sqrtf(y0 * y0 + y1 * y1 + y2 * y2);
  if (!len) {
    y0 = 0;
    y1 = 0;
    y2 = 0;
  } else {
    len = 1 / len;
    y0 *= len;
    y1 *= len;
    y2 *= len;
  }

  dest->raw[0] = x0;
  dest->raw[1] = y0;
  dest->raw[2] = z0;
  dest->raw[3] = 0;
  dest->raw[4] = x1;
  dest->raw[5] = y1;
  dest->raw[6] = z1;
  dest->raw[7] = 0;
  dest->raw[8] = x2;
  dest->raw[9] = y2;
  dest->raw[10] = z2;
  dest->raw[11] = 0;
  dest->raw[12] = -(x0 * eyex + x1 * eyey + x2 * eyez);
  dest->raw[13] = -(y0 * eyex + y1 * eyey + y2 * eyez);
  dest->raw[14] = -(z0 * eyex + z1 * eyey + z2 * eyez);
  dest->raw[15] = 1;

  return dest;
}
