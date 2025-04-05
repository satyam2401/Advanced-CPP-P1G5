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

void MatrixOps::multiply_mm_transposed_b(const double *matrixA, int rowsA, int colsA,
                                         const double *matrixB_transposed, int rowsB, int colsB,
                                         double *result) {
    // TODO: Implement logic for when matrixB is already transposed
}
