#include "3DViewer.h"

#include <glad/gl.h>

#include <check.h>
#include <stdlib.h>

START_TEST(parse_cube) {
  t_object obj = {0};
  bool parsed = parse_obj("models/test-cube.obj", &obj);
  ck_assert_msg(parsed, "parse_obj() returned false");

  bool correct_bbox = true;
  correct_bbox &= obj.bbox.x_min == -1;
  correct_bbox &= obj.bbox.x_max == 1;
  correct_bbox &= obj.bbox.y_min == -1;
  correct_bbox &= obj.bbox.y_max == 1;
  correct_bbox &= obj.bbox.z_min == -1;
  correct_bbox &= obj.bbox.z_max == 1;
  ck_assert_msg(correct_bbox, "Wrong bounding box");

  // TODO further checks
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
  // TODO
  ck_assert(false);
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

  srunner_run_all(sr, CK_NORMAL);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
