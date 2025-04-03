//
// Created by Satyam Saurabh on 02/04/25.
//

#include <iostream>
#include "MatrixOps.h"

void MatrixOps::multiply_mv_row_major(const double *matrix, int rows, int cols, const double *vector, double *result) {
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

void MatrixOps::multiply_mv_col_major(const double *matrix, int rows, int cols, const double *vector, double *result) {

}

void
MatrixOps::multiply_mm_naive(const double *matrixA, int rowsA, int colsA, const double *matrixB, int rowsB, int colsB,
                             double *result) {

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

    for (int i = 0; i < rowsA; ++i){
        for (int j = 0; j < colsB; ++j){
            double temp = 0.0;
            for (int k = 0; k < colsA; ++k){
                temp += matrixA[i * colsA + k] * matrixB_transposed[j * rowsB + k];
            }
            result[i * colsB + j] = temp;
        }
    }



}
