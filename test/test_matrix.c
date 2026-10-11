#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../matrix.h"

static void fill_matrix(matrix_t *A, double values[]) {
  int idx = 0;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      A->matrix[i][j] = values[idx++];
    }
  }
}

START_TEST(test_create_matrix_valid) {
  matrix_t matrix;
  int result = s21_create_matrix(3, 4, &matrix);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(matrix.rows, 3);
  ck_assert_int_eq(matrix.columns, 4);
  ck_assert_ptr_nonnull(matrix.matrix);
  for (int i = 0; i < 3; i++) {
    ck_assert_ptr_nonnull(matrix.matrix[i]);
  }
  s21_remove_matrix(&matrix);
}
END_TEST

START_TEST(test_create_matrix_null_result) {
  int result = s21_create_matrix(3, 4, NULL);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_create_matrix_negative_rows) {
  matrix_t matrix;
  int result = s21_create_matrix(-1, 4, &matrix);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_create_matrix_zero_size) {
  matrix_t matrix;
  int result = s21_create_matrix(0, 0, &matrix);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_remove_matrix_normal) {
  matrix_t A;
  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  s21_remove_matrix(&A);
  ck_assert_int_eq(A.rows, 0);
  ck_assert_int_eq(A.columns, 0);
  ck_assert_ptr_null(A.matrix);
}
END_TEST

START_TEST(test_remove_matrix_null_input) { s21_remove_matrix(NULL); }
END_TEST

START_TEST(test_eq_matrix_identical) {
  matrix_t A, B;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 2, &B), 0);

  double vals[] = {1.0, 2.0, 3.0, 4.0};
  fill_matrix(&A, vals);
  fill_matrix(&B, vals);

  ck_assert_int_eq(s21_eq_matrix(&A, &B), 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_different_values) {
  matrix_t A, B;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 2, &B), 0);

  A.matrix[0][0] = 1.0;
  B.matrix[0][0] = 1.00001;

  ck_assert_int_eq(s21_eq_matrix(&A, &B), 0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_different_sizes) {
  matrix_t A, B;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 3, &B), 0);

  ck_assert_int_eq(s21_eq_matrix(&A, &B), 0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_null_pointers) {
  matrix_t A;
  ck_assert_int_eq(s21_create_matrix(1, 1, &A), 0);

  ck_assert_int_eq(s21_eq_matrix(NULL, &A), 0);
  ck_assert_int_eq(s21_eq_matrix(&A, NULL), 0);
  ck_assert_int_eq(s21_eq_matrix(NULL, NULL), 0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_sum_matrix_valid) {
  matrix_t A, B, result;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 2, &B), 0);

  double a_vals[] = {1, 2, 3, 4};
  double b_vals[] = {5, 6, 7, 8};
  fill_matrix(&A, a_vals);
  fill_matrix(&B, b_vals);

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);
  ck_assert_ldouble_eq(result.matrix[0][0], 6.0);
  ck_assert_ldouble_eq(result.matrix[1][1], 12.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_different_sizes) {
  matrix_t A, B, result;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 3, &B), 0);

  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_matrix_null_pointers) {
  matrix_t A, B, result;
  ck_assert_int_eq(s21_create_matrix(1, 1, &A), 0);
  ck_assert_int_eq(s21_create_matrix(1, 1, &B), 0);

  ck_assert_int_eq(s21_sum_matrix(NULL, &B, &result), 1);
  ck_assert_int_eq(s21_sum_matrix(&A, NULL, &result), 1);
  ck_assert_int_eq(s21_sum_matrix(&A, &B, NULL), 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sub_matrix_valid) {
  matrix_t A, B, result;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 2, &B), 0);

  double a_vals[] = {5, 6, 7, 8};
  double b_vals[] = {1, 2, 3, 4};
  fill_matrix(&A, a_vals);
  fill_matrix(&B, b_vals);

  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), 0);
  ck_assert_ldouble_eq(result.matrix[0][0], 4.0);
  ck_assert_ldouble_eq(result.matrix[1][1], 4.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_valid) {
  matrix_t A, result;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  double vals[] = {1, 2, 3, 4};
  fill_matrix(&A, vals);

  ck_assert_int_eq(s21_mult_number(&A, 2.5, &result), 0);
  ck_assert_ldouble_eq(result.matrix[0][0], 2.5);
  ck_assert_ldouble_eq(result.matrix[1][1], 10.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_valid) {
  matrix_t A, B, result;
  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 2, &B), 0);

  double a_vals[] = {1, 2, 3, 4, 5, 6};
  double b_vals[] = {7, 8, 9, 10, 11, 12};
  fill_matrix(&A, a_vals);
  fill_matrix(&B, b_vals);

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), 0);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);
  ck_assert_ldouble_eq(result.matrix[0][0], 58.0);
  ck_assert_ldouble_eq(result.matrix[1][1], 154.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_incompatible) {
  matrix_t A, B, result;
  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 2, &B), 0);

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_transpose_valid) {
  matrix_t A, result;
  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  double vals[] = {1, 2, 3, 4, 5, 6};
  fill_matrix(&A, vals);

  ck_assert_int_eq(s21_transpose(&A, &result), 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 2);
  ck_assert_ldouble_eq(result.matrix[0][1], 4.0);
  ck_assert_ldouble_eq(result.matrix[2][0], 3.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_determinant_1x1) {
  matrix_t A;
  ck_assert_int_eq(s21_create_matrix(1, 1, &A), 0);
  A.matrix[0][0] = 5.0;

  double det = 0.0;
  ck_assert_int_eq(s21_determinant(&A, &det), 0);
  ck_assert_ldouble_eq(det, 5.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_2x2) {
  matrix_t A;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  double vals[] = {1, 2, 3, 4};
  fill_matrix(&A, vals);

  double det = 0.0;
  ck_assert_int_eq(s21_determinant(&A, &det), 0);
  ck_assert_ldouble_eq(det, -2.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_3x3) {
  matrix_t A;
  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  double vals[] = {6, 1, 1, 4, -2, 5, 2, 8, 7};
  fill_matrix(&A, vals);

  double det = 0.0;
  ck_assert_int_eq(s21_determinant(&A, &det), 0);
  ck_assert_ldouble_eq(det, -306.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_singular) {
  matrix_t A;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  double vals[] = {1, 2, 2, 4};
  fill_matrix(&A, vals);

  double det = 0.0;
  ck_assert_int_eq(s21_determinant(&A, &det), 0);
  ck_assert_ldouble_eq(det, 0.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_non_square) {
  matrix_t A;
  double det = 0.0;
  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  ck_assert_int_eq(s21_determinant(&A, &det), 2);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_2x2) {
  matrix_t A, result;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  double a_vals[] = {1, 2, 3, 4};
  fill_matrix(&A, a_vals);

  ck_assert_int_eq(s21_calc_complements(&A, &result), 0);
  ck_assert_ldouble_eq(result.matrix[0][0], 4.0);
  ck_assert_ldouble_eq(result.matrix[0][1], -3.0);
  ck_assert_ldouble_eq(result.matrix[1][0], -2.0);
  ck_assert_ldouble_eq(result.matrix[1][1], 1.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_non_square) {
  matrix_t A, result;
  ck_assert_int_eq(s21_create_matrix(3, 2, &A), 0);
  ck_assert_int_eq(s21_calc_complements(&A, &result), 2);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix_2x2) {
  matrix_t A, inv;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  double a_vals[] = {4, 7, 2, 6};
  fill_matrix(&A, a_vals);

  ck_assert_int_eq(s21_inverse_matrix(&A, &inv), 0);

  matrix_t identity;
  ck_assert_int_eq(s21_mult_matrix(&A, &inv, &identity), 0);
  ck_assert_ldouble_eq_tol(identity.matrix[0][0], 1.0, 1e-6);
  ck_assert_ldouble_eq_tol(identity.matrix[1][1], 1.0, 1e-6);
  ck_assert_ldouble_eq_tol(identity.matrix[0][1], 0.0, 1e-6);
  ck_assert_ldouble_eq_tol(identity.matrix[1][0], 0.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&inv);
  s21_remove_matrix(&identity);
}
END_TEST

START_TEST(test_inverse_matrix_singular) {
  matrix_t A, inv;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  double a_vals[] = {1, 2, 2, 4};
  fill_matrix(&A, a_vals);

  ck_assert_int_eq(s21_inverse_matrix(&A, &inv), 2);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_3x3_known) {
  matrix_t A, result;
  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  double a_vals[] = {1, 2, 3, 0, 4, 2, 5, 2, 1};
  fill_matrix(&A, a_vals);

  ck_assert_int_eq(s21_calc_complements(&A, &result), 0);
  ck_assert_ldouble_eq(result.matrix[0][0], 0.0);
  ck_assert_ldouble_eq(result.matrix[0][1], 10.0);
  ck_assert_ldouble_eq(result.matrix[0][2], -20.0);
  ck_assert_ldouble_eq(result.matrix[1][0], 4.0);
  ck_assert_ldouble_eq(result.matrix[1][1], -14.0);
  ck_assert_ldouble_eq(result.matrix[1][2], 8.0);
  ck_assert_ldouble_eq(result.matrix[2][0], -8.0);
  ck_assert_ldouble_eq(result.matrix[2][1], -2.0);
  ck_assert_ldouble_eq(result.matrix[2][2], 4.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_3x3_identity) {
  matrix_t A, result;
  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  double a_vals[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
  fill_matrix(&A, a_vals);

  ck_assert_int_eq(s21_calc_complements(&A, &result), 0);
  ck_assert_ldouble_eq(result.matrix[0][0], 1.0);
  ck_assert_ldouble_eq(result.matrix[0][1], 0.0);
  ck_assert_ldouble_eq(result.matrix[0][2], 0.0);
  ck_assert_ldouble_eq(result.matrix[1][0], 0.0);
  ck_assert_ldouble_eq(result.matrix[1][1], 1.0);
  ck_assert_ldouble_eq(result.matrix[1][2], 0.0);
  ck_assert_ldouble_eq(result.matrix[2][0], 0.0);
  ck_assert_ldouble_eq(result.matrix[2][1], 0.0);
  ck_assert_ldouble_eq(result.matrix[2][2], 1.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

Suite *s21_matrix_suite(void) {
  Suite *s = suite_create("s21_matrix");
  TCase *tc_core = tcase_create("Core");
  TCase *tc_eq = tcase_create("Eq");
  TCase *tc_arith = tcase_create("Arithmetic");
  TCase *tc_det = tcase_create("Determinant");
  TCase *tc_inv = tcase_create("Inverse");

  tcase_add_test(tc_core, test_create_matrix_valid);
  tcase_add_test(tc_core, test_create_matrix_null_result);
  tcase_add_test(tc_core, test_create_matrix_negative_rows);
  tcase_add_test(tc_core, test_create_matrix_zero_size);
  tcase_add_test(tc_core, test_remove_matrix_normal);
  tcase_add_test(tc_core, test_remove_matrix_null_input);

  tcase_add_test(tc_eq, test_eq_matrix_identical);
  tcase_add_test(tc_eq, test_eq_matrix_different_values);
  tcase_add_test(tc_eq, test_eq_matrix_different_sizes);
  tcase_add_test(tc_eq, test_eq_matrix_null_pointers);

  tcase_add_test(tc_arith, test_sum_matrix_valid);
  tcase_add_test(tc_arith, test_sum_matrix_different_sizes);
  tcase_add_test(tc_arith, test_sum_matrix_null_pointers);
  tcase_add_test(tc_arith, test_sub_matrix_valid);
  tcase_add_test(tc_arith, test_mult_number_valid);
  tcase_add_test(tc_arith, test_mult_matrix_valid);
  tcase_add_test(tc_arith, test_mult_matrix_incompatible);
  tcase_add_test(tc_arith, test_transpose_valid);

  tcase_add_test(tc_det, test_determinant_1x1);
  tcase_add_test(tc_det, test_determinant_2x2);
  tcase_add_test(tc_det, test_determinant_3x3);
  tcase_add_test(tc_det, test_determinant_singular);
  tcase_add_test(tc_det, test_determinant_non_square);
  tcase_add_test(tc_det, test_calc_complements_2x2);
  tcase_add_test(tc_det, test_calc_complements_non_square);

  tcase_add_test(tc_inv, test_inverse_matrix_2x2);
  tcase_add_test(tc_inv, test_inverse_matrix_singular);

  tcase_add_test(tc_det, test_calc_complements_3x3_known);
  tcase_add_test(tc_det, test_calc_complements_3x3_identity);

  suite_add_tcase(s, tc_core);
  suite_add_tcase(s, tc_eq);
  suite_add_tcase(s, tc_arith);
  suite_add_tcase(s, tc_det);
  suite_add_tcase(s, tc_inv);

  return s;
}

int main(void) {
  Suite *s = s21_matrix_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? 0 : 1;
}