#include "matrix.h"

#include <math.h>
#include <stdlib.h>

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int code = 0;
  if (result != NULL && rows > 0 && columns > 0) {
    result->rows = rows;
    result->columns = columns;
    result->matrix = calloc(rows, sizeof(double *));
    if (result->matrix != NULL) {
      for (int i = 0; i < rows; i++) {
        result->matrix[i] = calloc(columns, sizeof(double));
      }
    }
    code = 0;
  } else {
    if (result != NULL) {
      result->rows = 0;
      result->columns = 0;
    }
    code = 1;
  }
  return code;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL) {
    if (A->matrix != NULL) {
      for (int i = 0; i < A->rows; i++) {
        free(A->matrix[i]);
      }
      free(A->matrix);
      A->matrix = NULL;
    }
    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int code = SUCCESS;

  if (A == NULL || B == NULL) {
    code = FAILURE;
  } else if (A->matrix == NULL || B->matrix == NULL || A->rows <= 0 ||
             B->rows <= 0 || A->columns <= 0 || B->columns <= 0 ||
             A->rows != B->rows || A->columns != B->columns) {
    code = FAILURE;
  } else {
    for (int i = 0; i < A->rows && code == SUCCESS; i++) {
      for (int j = 0; j < A->columns && code == SUCCESS; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) >   1e-6) code = FAILURE;
      }
    }
  }
  return code;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code = 0;
  if (A == NULL || B == NULL || result == NULL || A->matrix == NULL ||
      B->matrix == NULL || A->rows <= 0 || B->rows <= 0 || A->columns <= 0 ||
      B->columns <= 0) {
    code = 1;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    code = 2;
  }
  if (code == 0 && s21_create_matrix(A->rows, A->columns, result) != 0) {
    code = 1;
  }
  if (code == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }
  return code;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code = 0;
  if (A == NULL || B == NULL || result == NULL || A->matrix == NULL ||
      B->matrix == NULL || A->rows <= 0 || B->rows <= 0 || A->columns <= 0 ||
      B->columns <= 0) {
    code = 1;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    code = 2;
  }
  if (code == 0 && s21_create_matrix(A->rows, A->columns, result) != 0) {
    code = 1;
  }
  if (code == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }
  return code;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int code = 0;
  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    code = 1;
  } else if (s21_create_matrix(A->rows, A->columns, result) != 0) {
    code = 1;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return code;
}

void s21_mult_matrix_on_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      result->matrix[i][j] = 0.0;
      for (int k = 0; k < A->columns; k++) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code = 0;

  if (A == NULL || B == NULL || A->matrix == NULL || B->matrix == NULL ||
      A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0) {
    code = 1;
  } else if (A->columns != B->rows) {
    code = 2;
  } else if (s21_create_matrix(A->rows, B->columns, result) != 0) {
    code = 1;
  } else {
    s21_mult_matrix_on_matrix(A, B, result);
  }
  return code;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int code = 0;

  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    code = 1;
  } else if (s21_create_matrix(A->columns, A->rows, result) != 0) {
    code = 1;
  }

  else {
    for (int i = 0; i < A->rows; ++i) {
      for (int j = 0; j < A->columns; ++j) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }
  return code;
}

static int s21_fill_minor_matrix(matrix_t *A, int del_row, int del_col,
                                 matrix_t *result) {
  int m_i = 0;
  for (int i = 0; i < A->rows; i++) {
    if (i != del_row) {
      int m_j = 0;
      for (int j = 0; j < A->rows; j++) {
        if (j != del_col) result->matrix[m_i][m_j++] = A->matrix[i][j];
      }
      m_i++;
    }
  }
  return 0;
}

int s21_minor_matrix(matrix_t *A, int delete_row, int delete_collum,
                     matrix_t *result) {
  int code = 0;
  int n = A->rows;
  int new_size = n - 1;
  if (A == NULL || A->matrix == NULL || result == NULL ||
      result->matrix == NULL) {
    code = 1;
  }
  if (code == 0) {
    if (A->columns != n || n < 1) {
      code = 1;
    }
  }
  if (code == 0) {
    if (result->rows != new_size && result->columns != new_size) {
      code = 1;
    }
  }
  if (code == 0) {
    if (delete_row < 0 || delete_row >= n || delete_collum < 0 ||
        delete_collum >= n) {
      code = 1;
    }
  }
  if (code == 0) {
    code = s21_fill_minor_matrix(A, delete_row, delete_collum, result);
  }
  return code;
}

static int s21_process_determinant_step(matrix_t *A, int col, double *result) {
  int code = 0;
  int n = A->rows;
  matrix_t minor = {0};
  code = s21_create_matrix(n - 1, n - 1, &minor);
  if (code == 0) {
    code = s21_minor_matrix(A, 0, col, &minor);
  }
  if (code == 0) {
    double sub_det = 0.0;
    code = s21_determinant(&minor, &sub_det);
    if (code == 0) {
      double sign = (col % 2 == 0) ? 1.0 : -1.0;
      *result += sign * A->matrix[0][col] * sub_det;
    }
  }
  s21_remove_matrix(&minor);
  return code;
}

int s21_determinant(matrix_t *A, double *result) {
  int code = 0;
  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    code = 1;
  } else if (A->rows != A->columns) {
    code = 2;
  }
  if (code == 0) {
    int n = A->rows;
    if (n == 1) {
      *result = A->matrix[0][0];
    } else if (n == 2) {
      *result =
          A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    } else {
      *result = 0.0;
      int n = A->rows;
      for (int j = 0; j < n && code == 0; j++) {
        code = s21_process_determinant_step(A, j, result);
      }
    }
  }
  return code;
}

static int s21_compute_complement_element(matrix_t *A, int row, int col,
                                          double *result) {
  int code = 0;
  int n = A->rows;
  matrix_t minor = {0};
  double det_minor = 0.0;
  code = s21_create_matrix(n - 1, n - 1, &minor);
  if (code == 0) {
    code = s21_minor_matrix(A, row, col, &minor);
  }
  if (code == 0) {
    code = s21_determinant(&minor, &det_minor);
  }
  if (code == 0) {
    double sign = ((row + col) % 2 == 0) ? +1.0 : -1.0;
    *result = sign * det_minor;
  }
  s21_remove_matrix(&minor);
  return code;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int code = 0;
  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    code = 1;
  } else if (A->rows != A->columns) {
    code = 2;
  }

  int n = 0;

  if (code == 0) {
    n = A->rows;
    code = s21_create_matrix(n, n, result);
  }

  if (code == 0) {
    if (n == 1) {
      result->matrix[0][0] = 1.0;
    } else if (n == 2) {
      result->matrix[0][0] = A->matrix[1][1];
      result->matrix[0][1] = -A->matrix[1][0];
      result->matrix[1][0] = -A->matrix[0][1];
      result->matrix[1][1] = A->matrix[0][0];
    } else {
      for (int i = 0; i < n && code == 0; i++) {
        for (int j = 0; j < n && code == 0; j++)
          code = s21_compute_complement_element(A, i, j, &result->matrix[i][j]);
      }
    }
  }
  return code;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int code = 0;
  double det = 0.0;
  matrix_t complements = {0};
  matrix_t transposed = {0};
  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    code = 1;
  } else if (A->rows != A->columns) {
    code = 2;
  }
  if (code == 0) {
    code = s21_determinant(A, &det);
  }
  if (code == 0) {
    if (fabs(det) < 1e-9) {
      code = 2;
    } else {
      code = s21_calc_complements(A, &complements);
    }
    if (code == 0) {
      code = s21_transpose(&complements, &transposed);
    }
    if (code == 0) {
      code = s21_mult_number(&transposed, 1.0 / det, result);
    }
  }
  s21_remove_matrix(&complements);
  s21_remove_matrix(&transposed);

  return code;
}