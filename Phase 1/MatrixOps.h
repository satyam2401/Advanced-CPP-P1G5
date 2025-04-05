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

    inline static void multiply_mv_col_major_inline(const double *matrix, int rows, int cols,
                                                  const double *vector, double *result) {
        if (!matrix || !vector || !result) {
            std::cerr << "[ERROR] Null pointer passed to multiply_mv_col_major_inline.\n";
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

    // Matrix-Matrix Multiplication (Naive, Row-Major)
    static void multiply_mm_naive(const double *matrixA, int rowsA, int colsA,
                                  const double *matrixB, int rowsB, int colsB,
                                  double *result);

    inline static void multiply_mm_naive_inline(const double *matrixA, int rowsA, int colsA, const double *matrixB, 
                                        int rowsB, int colsB, double *result) {

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

                }
            result[colsB * rowA + colB] = dotResult;
            }
        }
    return;
    };

    // Matrix-Matrix Multiplication with Transposed B (both in Row-Major)
    static void multiply_mm_transposed_b(const double *matrixA, int rowsA, int colsA,
                                         const double *matrixB_transposed, int rowsB, int colsB,
                                         double *result);
};

#endif ADVANCED_CPP_P1G5_MATRIXOPS_H
