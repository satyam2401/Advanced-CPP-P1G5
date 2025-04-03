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

void run_benchmarks_matrix_vector_multiplication() {
    benchmark_matrix_vector_multiplication(10, 10);       // Small
    benchmark_matrix_vector_multiplication(500, 500);     // Medium
    benchmark_matrix_vector_multiplication(1000, 1000);   // Large
}
