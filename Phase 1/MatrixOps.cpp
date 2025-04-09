//
// Created by Satyam Saurabh on 02/04/25.
//

#include <iostream>
#include "MatrixOps.h"

void MatrixOps::multiply_mv_row_major(const double *matrix, int rows, int cols,
                                      const double *vector, double *result) {
    if (!matrix || !vector || !result) {
        std::cerr << "[ERROR] Null pointer passed to multiply_mv_row_major.\n";
        return;
    }

    if (rows <= 0 || cols <= 0) {
        std::cerr << "[ERROR] Invalid matrix dimensions.\n";
        return;
    }

    for(int row = 0; row < rows; ++row){
        double temp = 0.0;
        for(int col = 0; col < cols; ++col){
            temp += matrix[row * cols + col] * vector[col];
        }
        result[row] = temp;
    }
}

inline double dot_product_unrolled_aligned(const double* row, const double* vector, int cols) {
    double sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
    int i = 0;

    // Loop unrolling by 4 for better ILP and vectorization
    for (; i <= cols - 4; i += 4) {
        sum0 += row[i]     * vector[i];
        sum1 += row[i + 1] * vector[i + 1];
        sum2 += row[i + 2] * vector[i + 2];
        sum3 += row[i + 3] * vector[i + 3];
    }

    double total = sum0 + sum1 + sum2 + sum3;

    // Handle remaining elements
    for (; i < cols; ++i) {
        total += row[i] * vector[i];
    }

    return total;
}

void MatrixOps::multiply_mv_row_major_optimised(const double* matrix, int rows, int cols,
                                     const double* vector, double* result) {
    if (!matrix || !vector || !result) {
        std::cerr << "[ERROR] Null pointer passed to multiply_mv_row_major_optimised.\n";
        return;
    }

    if (rows <= 0 || cols <= 0) {
        std::cerr << "[ERROR] Invalid matrix dimensions.\n";
        return;
    }

    for (int row = 0; row < rows; ++row) {
        const double* row_ptr = matrix + row * cols;
        result[row] = dot_product_unrolled_aligned(row_ptr, vector, cols);
    }
}

void MatrixOps::multiply_mv_col_major(const double *matrix, int rows, int cols,
                                      const double *vector, double *result) {
    if (!matrix || !vector || !result) {
        std::cerr << "[ERROR] Null pointer passed to multiply_mv_col_major.\n";
        return;
    }

    if (rows <= 0 || cols <= 0) {
        std::cerr << "[ERROR] Invalid matrix dimensions.\n";
        return;
    }

    for (int row = 0; row < rows; ++row) {
        result[row] = 0.0;
    }

    for (int col = 0; col < cols; ++col) {
        for (int row = 0; row < rows; ++row) {
            result[row] += matrix[col * rows + row] * vector[col];
        }
    }
}

void MatrixOps::multiply_mv_col_major_inline(const double *matrix, int rows, int cols,
                                             const double *vector, double *result) {
    if (!matrix || !vector || !result) {
        std::cerr << "[ERROR] Null pointer passed to multiply_mv_col_major_inline.\n";
        return;
    }

    if (rows <= 0 || cols <= 0) {
        std::cerr << "[ERROR] Invalid matrix dimensions.\n";
        return;
    }

    for (int row = 0; row < rows; ++row) {
        result[row] = 0.0;
    }

    for (int col = 0; col < cols; ++col) {
        for (int row = 0; row < rows; ++row) {
            result[row] += matrix[col * rows + row] * vector[col];
        }
    }
}

void MatrixOps::multiply_mm_naive(const double *matrixA, int rowsA, int colsA,
                                  const double *matrixB, int rowsB, int colsB,
                                  double *result) {
    if (!matrixA || !matrixB || !result) {
        std::cerr << "[ERROR] Null pointer passed to multiply_mm_naive.\n";
        return;
    }

    if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0) {
        std::cerr << "[ERROR] Invalid matrix dimensions.\n";
        return;
    }

    if (colsA != rowsB) {
        std::cerr << "[ERROR] Incompatible matrix dimensions for multiplication.\n";
        return;
    }

    for (int rowA = 0; rowA < rowsA; ++rowA) {
        for (int colB = 0; colB < colsB; ++colB) {
            double dotResult = 0.0;
            for (int colA = 0; colA < colsA; ++colA) {
                dotResult += matrixA[colsA * rowA + colA] * matrixB[colsB * colA + colB];
            }
            result[colsB * rowA + colB] = dotResult;
        }
    }
}

void MatrixOps::multiply_mm_naive_inline(const double *matrixA, int rowsA, int colsA,
                                         const double *matrixB, int rowsB, int colsB,
                                         double *result) {
    if (!matrixA || !matrixB || !result) {
        std::cerr << "[ERROR] Null pointer passed to multiply_mm_naive_inline.\n";
        return;
    }

    if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0) {
        std::cerr << "[ERROR] Invalid matrix dimensions.\n";
        return;
    }
}

void MatrixOps::multiply_mm_transposed_b(const double *matrixA, int rowsA, int colsA, const double *matrixB_transposed,
                                         int rowsB, int colsB, double *result) {
    if (!matrixA || !matrixB_transposed || !result) {
        std::cerr << "[ERROR] Null pointer passed to multiply_mm_transposed_b.\n";
        return;
    }

    if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0) {
        std::cerr << "[ERROR] Invalid matrix dimensions.\n";
        return;
    }

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            double temp = 0.0;
            for (int k = 0; k < colsA; ++k) {
                temp += matrixA[i * colsA + k] * matrixB_transposed[j * rowsB + k];
            }
            result[i * colsB + j] = temp;
        }
    }


    return;
}

inline double multiply_element(const double *matrixA,const double *matrixB, int i, int j, int k, int colsA, int rowsB){
    return matrixA[colsA * i + k] * matrixB[rowsB * j + k];
}

void MatrixOps::multiply_mm_transposed_b_inline(const double *matrixA, int rowsA, int colsA, const double *matrixB_transposed,
                                         int rowsB, int colsB, double *result) {
    if (!matrixA || !matrixB_transposed || !result) {
        std::cerr << "[ERROR] Null pointer passed to multiply_mm_transposed_b.\n";
        return;
    }

    if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0) {
        std::cerr << "[ERROR] Invalid matrix dimensions.\n";
        return;
    }

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            double temp = 0.0;
            for (int k = 0; k < colsA; ++k) {
                temp += multiply_element(matrixA, matrixB_transposed, i, j, k, colsA, rowsB);
            }
            result[i * colsB + j] = temp;
        }
    }


    return;
}