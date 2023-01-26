#include "3DViewer.h"
#include "vector.h"

#include <glad/gl.h>

#include <check.h>

#define array_compare(a, b, len, result)                                       \
  do {                                                                         \
    *(result) = true;                                                          \
    for (size_t i = 0; i < (len); ++i) {                                       \
      if ((a)[i] != (b)[i]) {                                                  \
        *(result) = false;                                                     \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  } while (0)


START_TEST(parse_cube) {
  t_object obj = {0};
  bool parsed = parse_obj("models/test-cube.obj", &obj);
  ck_assert_msg(parsed, "parse_obj() returned false");

  bool equal = true;
  float correct_bbox[6] = {-1, 1, -1, 1, -1, 1};
  array_compare(correct_bbox, obj.bbox.raw, 6, &equal);
  ck_assert_msg(equal, "Wrong bounding box");

  ck_assert(obj.vertex_count == 8);
  ck_assert(obj.edge_count == 12);

  // clang-format off
  GLfloat correct_vertices[27] = {
    // a dummy vertex
     0,  0,  0,
    // front face
     1,  1, -1, // 0 top right
     1, -1, -1, // 1 bottom right
    -1, -1, -1, // 2 bottom left
    -1,  1, -1, // 3 top left
    // back face
     1,  1,  1, // 4 top right
     1, -1,  1, // 5 bottom right
    -1, -1,  1, // 6 bottom left
    -1,  1,  1  // 7 top left
  };
  // clang-format on
  array_compare(correct_vertices, obj.vertices, 27, &equal);
  ck_assert_msg(equal, "Wrong vertices");

  GLuint correct_indices[48] = {
      1, 2, 2, 3, 3, 4, 4, 1, // front face
      5, 6, 6, 7, 7, 8, 8, 5, // back face
      3, 4, 4, 8, 8, 7, 7, 3, // left face
      1, 2, 2, 6, 6, 5, 5, 1, // right face
      1, 5, 5, 8, 8, 4, 4, 1, // top face
      2, 6, 6, 7, 7, 3, 3, 2, // bottom face
  };
  array_compare(correct_indices, obj.indices, 48, &equal);
  ck_assert_msg(equal, "Wrong indices");
}
END_TEST

START_TEST(parse_cube_invalid) {
  t_object obj = {0};
  bool parsed = parse_obj("models/test-cube-invalid.obj", &obj);
  ck_assert_msg(!parsed, "parse_obj() returned true for an invalid file");
  ck_assert(obj.vertex_count == 0);
  ck_assert(obj.edge_count == 0);
}
END_TEST

START_TEST(parse_non_existent) {
  t_object obj = {0};
  bool parsed = parse_obj("models/non-existent-file.obj", &obj);
  ck_assert_msg(!parsed, "parse_obj() returned true for a non-existent file");
}
END_TEST

START_TEST(translate) {
  t_mat4 m = mat4_create_identity();
  t_mat4 dest;
  mat4_translate(&m, &(t_vec3){{-1, 2, 3}}, &dest);

  bool equal = true;
  float correct_dest[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -1, 2, 3, 1};
  array_compare(correct_dest, dest.raw, 16, &equal);
  ck_assert_msg(equal, "Wrong translation matrix");
}
END_TEST

START_TEST(rotateX) {
  t_mat4 m = mat4_create_identity();
  t_mat4 dest;
  mat4_rotateX(&m, 123, &dest);

  bool equal = true;
  // clang-format off
  float correct_dest[16] = {
    1, 0, 0, 0, -0, -0.88796889781951904296875f, -0.4599034786224365234375f, -0,
    0, 0.4599034786224365234375f, -0.88796889781951904296875f, 0, 0, 0, 0, 1
  };
  // clang-format on
  array_compare(correct_dest, dest.raw, 16, &equal);
  ck_assert_msg(equal, "Wrong rotation matrix");
}
END_TEST

START_TEST(rotateY) {
  t_mat4 m = mat4_create_identity();
  t_mat4 dest;
  mat4_rotateY(&m, 42, &dest);

  bool equal = true;
  // clang-format off
  float correct_dest[16] = {
    -0.39998531341552734375f, 0, 0.916521549224853515625f, 0, -0, 1, 0, -0,
    -0.916521549224853515625f, -0, -0.39998531341552734375f, 0, 0, 0, 0, 1
  };
  // clang-format on
  array_compare(correct_dest, dest.raw, 16, &equal);
  ck_assert_msg(equal, "Wrong rotation matrix");
}
END_TEST

START_TEST(rotateZ) {
  t_mat4 m = mat4_create_identity();
  t_mat4 dest;
  mat4_rotateZ(&m, -69, &dest);

  bool equal = true;
  // clang-format off
  float correct_dest[16] = {
    0.993390381336212158203125f, 0.114784814417362213134765625f, 0, 0,
    -0.114784814417362213134765625f, 0.993390381336212158203125f, 0, -0, 0, 0,
    1, 0, 0, 0, 0, 1
  };
  // clang-format on
  array_compare(correct_dest, dest.raw, 16, &equal);
  ck_assert_msg(equal, "Wrong rotation matrix");
}
END_TEST

START_TEST(scale) {
  t_mat4 m = mat4_create_identity();
  t_mat4 dest;
  mat4_scale_float(&m, 420, &dest);

  bool equal = true;
  // clang-format off
  float correct_dest[16] = {
    420, 0,   0,   0,
    0,   420, 0,   0,
    0,   0,   420, 0,
    0,   0,   0,   1
  };
  // clang-format on
  array_compare(correct_dest, dest.raw, 16, &equal);
  ck_assert_msg(equal, "Wrong scale matrix");
}
END_TEST

START_TEST(multiple_transforms) {
  t_mat4 m = mat4_create_identity();
  t_mat4 dest;
  mat4_translate(&m, &(t_vec3){{1, -2, -3}}, NULL);
  mat4_rotateX(&m, 12, NULL);
  mat4_rotateY(&m, -23, NULL);
  mat4_rotateZ(&m, 34, NULL);
  mat4_scale_float(&m, 3, &dest);

  bool equal = true;
  // clang-format off
  float correct_dest[16] = {
    1.3564388751983642578125f, 2.495308399200439453125f,
    0.966183364391326904296875f, 0, 0.8457381725311279296875f,
    -1.427504062652587890625f, 2.4993917942047119140625f, 0,
    2.5386612415313720703125f, -0.857711315155029296875f, -1.34889984130859375f,
    0, 1, -2, -3, 1
  };
  // clang-format on
  array_compare(correct_dest, dest.raw, 16, &equal);
  ck_assert_msg(equal, "Wrong transform matrix");
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("3DViewer");
  TCase *tc1_1 = tcase_create("File Loader");
  TCase *tc1_2 = tcase_create("Affine Transformations");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, parse_cube);
  tcase_add_test(tc1_1, parse_cube_invalid);
  tcase_add_test(tc1_2, parse_non_existent);

  suite_add_tcase(s1, tc1_2);
  tcase_add_test(tc1_2, translate);
  tcase_add_test(tc1_2, rotateX);
  tcase_add_test(tc1_2, rotateY);
  tcase_add_test(tc1_2, rotateZ);
  tcase_add_test(tc1_2, scale);
  tcase_add_test(tc1_2, multiple_transforms);

  srunner_run_all(sr, CK_NORMAL);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
