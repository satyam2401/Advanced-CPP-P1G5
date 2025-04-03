#include <iostream>
#include <chrono>
#include <cmath>
#include "../Phase 1/MatrixOps.h"

void benchmark_matrix_vector_multiplication(int rows, int cols, int runs = 25) {
    std::cout << "\n[Benchmark] Size: " << rows << "x" << cols << " | Runs: " << runs << "\n";

    auto* matrix = new double[rows * cols];
    auto* vector = new double[cols];
    auto* result = new double[rows];

    // Fill matrix and vector with dummy data
    for (int i = 0; i < rows * cols; ++i) matrix[i] = static_cast<double>((i % 10) + 1);
    for (int i = 0; i < cols; ++i) vector[i] = static_cast<double>((i % 5) + 1);

    // Store timings
    double total_time = 0.0;
    // double times[runs];
    double times[25];

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

void run_benchmarks_matrix_vector_multiplication() {
    benchmark_matrix_vector_multiplication(10, 10);       // Small
    benchmark_matrix_vector_multiplication(500, 500);     // Medium
    benchmark_matrix_vector_multiplication(1000, 1000);   // Large
}

void benchmark_mm_naive_multiplication(int rowsA, int colsA, int rowsB, int colsB, int runs = 25) {
    std::cout << "\n[Benchmark] SizeA: " << rowsA << "x" << colsA << " | Runs: " << runs << "\n";
    std::cout << "\n[Benchmark] SizeB: " << rowsB << "x" << colsB << " | Runs: " << runs << "\n";

    auto* matrixA = new double[rowsA * colsA];
    auto* matrixB = new double[rowsB * colsB];
    auto* result = new double[rowsA * colsB];

    // Fill matrix A and B with dummy data
    for (int i = 0; i < rowsA * colsA; ++i) matrixA[i] = 1.0;
    for (int i = 0; i < rowsA * colsB; ++i) matrixB[i] = 1.0;

    // Store timings
    double total_time = 0.0;
    // double times[runs];
    double times[25];

    for (int r = 0; r < runs; ++r) {
        auto start = std::chrono::high_resolution_clock::now();
        MatrixOps::multiply_mm_naive(matrixA, rowsA, colsA, matrixB, rowsB, colsB, result);
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
    delete[] matrixB;
    delete[] result;
}

void run_benchmarks_mm_naive_multiplication() {
    benchmark_mm_naive_multiplication(10, 10, 10, 10);       // Small
    benchmark_mm_naive_multiplication(500, 500, 500, 500);     // Medium
    benchmark_mm_naive_multiplication(1000, 1000, 1000, 1000);   // Large
}
