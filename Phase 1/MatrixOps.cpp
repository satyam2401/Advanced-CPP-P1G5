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
    return;
}

void MatrixOps::multiply_mv_col_major(const double *matrix, int rows, int cols, const double *vector, double *result) {

    if (!matrix || !vector || !result) {
        std::cerr << "[ERROR] Null pointer passed to multiply_mv_col_major.\n";
        return;
    }

    if (rows <= 0 || cols <= 0) {
        std::cerr << "[ERROR] Invalid matrix dimensions.\n";
        return;
    }

    // Initialize result array with zeros
    for (int row = 0; row < rows; ++row) {
        result[row] = 0.0;
    }

    // For each column
    for (int col = 0; col < cols; ++col) {
        // For each row
        for (int row = 0; row < rows; ++row) {
            result[row] += matrix[col * rows + row] * vector[col];
        }
    }

    return;
}

void MatrixOps::multiply_mm_naive(const double *matrixA, int rowsA, int colsA, const double *matrixB, int rowsB, int colsB,
                             double *result) {
        if (!matrixA || !matrixB || !result) {
            std::cerr << "[ERROR] Null pointer passed to multiply_mm_naive.\n";
            return;
        }
                            
        if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <=0) {
            std::cerr << "[ERROR] Invalid matrix dimensions.\n";
            return;
        }

        if (colsA != rowsB) {
            std::cerr << "[ERROR] Invalid Matrix dimensions.\n";
            return;
        }

        // each row of A gets dotted with the corresponding col of B (do not transpose B or make a new one)
        for (int rowA = 0; rowA < rowsA; ++rowA){
            for (int colB = 0; colB < colsB; ++colB){
                double dotResult = 0.0;
                for (int colA = 0; colA < colsA; ++colA){
                    dotResult += matrixA[colsA * rowA + colA] * matrixB[colsB * colA + colB];
                    // whoever is doing the next function, code is identical. Switch this line with:
                    // dotResult += matrixA[colsA * rowA + colA] * matrixB[colsB * colB + colA];
                }
                result[colsB * rowA + colB] = dotResult;
            }
        }

        return;
}

void MatrixOps::multiply_mm_transposed_b(const double *matrixA, int rowsA, int colsA, const double *matrixB_transposed,
                                         int rowsB, int colsB, double *result) {
    return;
}
