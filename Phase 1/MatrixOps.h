//
// Created by Satyam Saurabh on 02/04/25.
//

#ifndef ADVANCED_CPP_P1G5_MATRIXOPS_H
#define ADVANCED_CPP_P1G5_MATRIXOPS_H

class MatrixOps {
public:
    // Matrix-Vector Multiplication (Row-Major)
    static void multiply_mv_row_major(const double *matrix, int rows, int cols,
                                      const double *vector, double *result);

    // Matrix-Vector Multiplication (Column-Major)
    static void multiply_mv_col_major(const double *matrix, int rows, int cols,
                                      const double *vector, double *result);

    // Matrix-Matrix Multiplication (Naive, Row-Major)
    static void multiply_mm_naive(const double *matrixA, int rowsA, int colsA,
                                  const double *matrixB, int rowsB, int colsB,
                                  double *result);

    // Matrix-Matrix Multiplication with Transposed B (both in Row-Major)
    static void multiply_mm_transposed_b(const double *matrixA, int rowsA, int colsA,
                                         const double *matrixB_transposed, int rowsB, int colsB,
                                         double *result);
};

#endif ADVANCED_CPP_P1G5_MATRIXOPS_H
