#include <iostream>
#include <chrono>
#include <cmath>
#include "../Phase 1/MatrixOps.h"

void benchmark_matrix_vector_multiplication(int rows, int cols, int runs = 5) {
    std::cout << "\n[Benchmark] Size: " << rows << "x" << cols << " | Runs: " << runs << "\n";

    auto* matrix = new double[rows * cols];
    auto* vector = new double[cols];
    auto* result = new double[rows];

    // Fill matrix and vector with dummy data
    for (int i = 0; i < rows * cols; ++i) matrix[i] = static_cast<double>((i % 10) + 1);
    for (int i = 0; i < cols; ++i) vector[i] = static_cast<double>((i % 5) + 1);

    // Store timings
    double total_time = 0.0;
    double times[runs];

    for (int r = 0; r < runs; ++r) {
        auto start = std::chrono::high_resolution_clock::now();
        MatrixOps::multiply_mv_row_major(matrix, rows, cols, vector, result);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> duration = end - start;
        times[r] = duration.count();
        total_time += times[r];
    }

    // Compute average
    double avg = total_time / runs;

    // Compute std deviation
    double variance = 0.0;
    for (int r = 0; r < runs; ++r)
        variance += (times[r] - avg) * (times[r] - avg);
    variance /= runs;
    double stddev = std::sqrt(variance);

    std::cout << "Average Time: " << avg << " µs\n";
    std::cout << "Std Dev:      " << stddev << " µs\n";

    delete[] matrix;
    delete[] vector;
    delete[] result;
}

void bechmarks_matrix_matrixtranspose_multiplications(int rowsA, int colsA, int rowsB, int colsB, int runs = 5){
    std::cout << "\n[Benchmark] Size: " << rowsA << "x" << colsA << " | " << rowsB << "x" << colsB << " | Runs: " << runs << "\n";

    auto* matrixA = new double[rowsA * colsA];
    auto* matrixB_transpose = new double[colsB * rowsB];
    auto* result = new double[rowsA * colsB];

    // Fill matrices with dummy data
    for (int i = 0; i < rowsA * colsA; ++i) matrixA[i] = static_cast<double>((i % 10) + 1);
    for (int i = 0; i < rowsB * colsB; ++i) matrixB_transpose[i] = static_cast<double>((i % 10) + 1);

    // Store timings
    double total_time = 0.0;
    double times[runs];

    for (int r = 0; r < runs; ++r) {
        auto start = std::chrono::high_resolution_clock::now();
        MatrixOps::multiply_mm_transposed_b(matrixA, rowsA, colsA, matrixB_transpose, rowsB, colsB, result);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> duration = end - start;
        times[r] = duration.count();
        total_time += times[r];
    }

    // Compute average
    double avg = total_time / runs;

    // Compute std deviation
    double variance = 0.0;
    for (int r = 0; r < runs; ++r)
        variance += (times[r] - avg) * (times[r] - avg);
    variance /= runs;
    double stddev = std::sqrt(variance);

    std::cout << "Average Time: " << avg << " µs\n";
    std::cout << "Std Dev:      " << stddev << " µs\n";

    delete[] matrixA;
    delete[] matrixB_transpose;
    delete[] result;
}

void run_benchmarks_matrix_vector_multiplication() {
    benchmark_matrix_vector_multiplication(10, 10);       // Small
    benchmark_matrix_vector_multiplication(500, 500);     // Medium
    benchmark_matrix_vector_multiplication(1000, 1000);   // Large
}

void run_benchmarks_matrix_matrixtranposed_multiplication() {
    bechmarks_matrix_matrixtranspose_multiplications(10, 10, 10, 10);       // Small
    bechmarks_matrix_matrixtranspose_multiplications(500, 500, 500, 500);   // Medium
    bechmarks_matrix_matrixtranspose_multiplications(1000, 1000, 1000, 1000); // Large
}