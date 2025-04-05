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

    std::cout << "Average Time: " << avg << " us\n";
    std::cout << "Std Dev:      " << stddev << " us\n";

    delete[] matrix;
    delete[] vector;
    delete[] result;
}

void run_benchmarks_matrix_vector_multiplication() {
    benchmark_matrix_vector_multiplication(10, 10);       // Small
    benchmark_matrix_vector_multiplication(500, 500);     // Medium
    benchmark_matrix_vector_multiplication(1000, 1000);   // Large
}

void benchmark_matrix_vector_col_major_multiplication(int rows, int cols, int runs = 25) {
    std::cout << "\n[Benchmark] Column-Major MV | Size: " << rows << "x" << cols << " | Runs: " << runs << "\n";

    auto* matrix = new double[rows * cols];
    auto* vector = new double[cols];
    auto* result = new double[rows];

    // Fill matrix and vector with dummy data

    for (int col = 0; col < cols; ++col) {
        for (int row = 0; row < rows; ++row) {
            matrix[col * rows + row] = static_cast<double>((col * rows + row) % 10 + 1);
        }
    }

    for (int i = 0; i < cols; ++i) {
        vector[i] = static_cast<double>((i % 5) + 1);
    }

    // Store timings
    double total_time = 0.0;
    double times[25];

    for (int r = 0; r < runs; ++r) {
        auto start = std::chrono::high_resolution_clock::now();
        MatrixOps::multiply_mv_col_major(matrix, rows, cols, vector, result);
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

    std::cout << "Average Time: " << avg << " us\n";
    std::cout << "Std Dev:      " << stddev << " us\n";

    delete[] matrix;
    delete[] vector;
    delete[] result;
}

void run_benchmarks_matrix_vector_col_major_multiplication() {
    benchmark_matrix_vector_col_major_multiplication(10, 10);       // Small
    benchmark_matrix_vector_col_major_multiplication(500, 500);     // Medium
    benchmark_matrix_vector_col_major_multiplication(1000, 1000);   // Large
}

void benchmark_matrix_vector_col_major_with_strides(int size, int runs = 10) {
    int strides[] = {1, 2, 4, 8, 16, 32};

    std::cout << "\n[Cache Locality Benchmark] Testing Column-Major MV with different strides\n";
    std::cout << "Matrix Size: " << size << "x" << size << " | Runs per stride: " << runs << "\n";

    for (int stride_idx = 0; stride_idx < 6; ++stride_idx) {
        int stride = strides[stride_idx];

        int padded_rows = size * stride;
        auto* matrix = new double[padded_rows * size];
        auto* vector = new double[size];
        auto* result = new double[size];

        for (int i = 0; i < padded_rows * size; ++i) matrix[i] = 0.0;

        for (int col = 0; col < size; ++col) {
            for (int row = 0; row < size; ++row) {
                matrix[col * padded_rows + row * stride] = static_cast<double>((col * size + row) % 10 + 1);
            }
        }

        for (int i = 0; i < size; ++i) {
            vector[i] = static_cast<double>((i % 5) + 1);
        }

        auto strided_mv_col_major = [padded_rows, stride](const double* matrix, int rows, int cols,
                                                        const double* vector, double* result) {
            for (int row = 0; row < rows; ++row) {
                result[row] = 0.0;
            }

            for (int col = 0; col < cols; ++col) {
                for (int row = 0; row < rows; ++row) {
                    result[row] += matrix[col * padded_rows + row * stride] * vector[col];
                }
            }
        };

        double total_time = 0.0;
        double times[25];

        for (int r = 0; r < runs; ++r) {
            auto start = std::chrono::high_resolution_clock::now();

            strided_mv_col_major(matrix, size, size, vector, result);

            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::micro> duration = end - start;
            times[r] = duration.count();
            total_time += times[r];
        }

        double avg = total_time / runs;

        double variance = 0.0;
        for (int r = 0; r < runs; ++r)
            variance += (times[r] - avg) * (times[r] - avg);
        variance /= runs;
        double stddev = std::sqrt(variance);

        std::cout << "Stride: " << stride << " | Average Time: " << avg << " us | Std Dev: " << stddev << " us\n";

        delete[] matrix;
        delete[] vector;
        delete[] result;
    }
}

void benchmark_mm_naive_multiplication(int rowsA, int colsA, int rowsB, int colsB, int runs = 25) {
    std::cout << "\n[Benchmark] SizeA: " << rowsA << "x" << colsA << " | Runs: " << runs << "\n";
    std::cout << "\n[Benchmark] SizeB: " << rowsB << "x" << colsB << " | Runs: " << runs << "\n";

    auto* matrixA = new double[rowsA * colsA];
    auto* matrixB = new double[rowsB * colsB];
    auto* result = new double[rowsA * colsB];

    // Fill matrix A and B with dummy data
    for (int i = 0; i < rowsA * colsA; ++i) matrixA[i] = 1.0;
    for (int i = 0; i < rowsB * colsB; ++i) matrixB[i] = 1.0;

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

    std::cout << "Average Time: " << avg << " us\n";
    std::cout << "Std Dev:      " << stddev << " us\n";

    delete[] matrixA;
    delete[] matrixB;
    delete[] result;
}

void run_benchmarks_mm_naive_multiplication() {
    benchmark_mm_naive_multiplication(10, 10, 10, 10);       // Small
    benchmark_mm_naive_multiplication(500, 500, 500, 500);     // Medium
    benchmark_mm_naive_multiplication(1000, 1000, 1000, 1000);   // Large
}

void benchmark_mm_naive_with_strides(int size, int runs = 10) {
    int strides[] = {1, 2, 4, 8, 16, 32};

    std::cout << "\n[Cache Locality Benchmark] Testing MM naive with different strides\n";
    std::cout << "Matrix Size: " << size << "x" << size << " | Runs per stride: " << runs << "\n";

    for (int stride_idx = 0; stride_idx < 6; ++stride_idx) {
        int stride = strides[stride_idx];

        int padded_rows = size * stride;
        auto* matrixA = new double[padded_rows * size];
        auto* matrixB = new double[padded_rows * size];
        auto* result = new double[padded_rows * size];

        for (int i = 0; i < padded_rows * size; ++i) matrixA[i] = 1.0;
        for (int i = 0; i < padded_rows * size; ++i) matrixB[i] = 1.0;

        for (int col = 0; col < size; ++col) {
            for (int row = 0; row < size; ++row) {
                matrixA[col * padded_rows + row * stride] = static_cast<double>(1);
            }
        }

        for (int col = 0; col < size; ++col) {
            for (int row = 0; row < size; ++row) {
                matrixB[col * padded_rows + row * stride] = static_cast<double>(1);
            }
        }

        auto strided_mm_naive = [padded_rows, stride](const double* matrixA, const double* matrixB, int rowsA, 
                                                        int colsA, int rowsB, int colsB, double* result) {
            // for (int row = 0; row < rows; ++row) {
            //     result[row] = 0.0;
            // }

            // for (int col = 0; col < cols; ++col) {
            //     for (int row = 0; row < rows; ++row) {
            //         result[row] += matrix[col * padded_rows + row * stride] * vector[col];
            //     }
            // }

            for (int rowA = 0; rowA < rowsA; ++rowA){
                for (int colB = 0; colB < colsB; ++colB){
                    double dotResult = 0.0;
                    for (int colA = 0; colA < colsA; ++colA){
                        dotResult += matrixA[colsA * (rowA*stride) + (colA*stride)] * matrixB[colsB * (colA*stride) + (colB*stride)];
                    }
                    // this is to maintain shape and to put the results in the right spots.
                    result[colsB * (rowA*stride) + (colB*stride)] = dotResult;
                }
            }
        };

        double total_time = 0.0;
        double times[25];

        for (int r = 0; r < runs; ++r) {
            auto start = std::chrono::high_resolution_clock::now();

            strided_mm_naive(matrixA, matrixB, size, size, size, size, result);

            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::micro> duration = end - start;
            times[r] = duration.count();
            total_time += times[r];
        }

        double avg = total_time / runs;

        double variance = 0.0;
        for (int r = 0; r < runs; ++r)
            variance += (times[r] - avg) * (times[r] - avg);
        variance /= runs;
        double stddev = std::sqrt(variance);

        std::cout << "Stride: " << stride << " | Average Time: " << avg << " us | Std Dev: " << stddev << " us\n";

        delete[] matrixA;
        delete[] matrixB;
        delete[] result;
    }
}

void run_cache_locality_benchmarks() {
    benchmark_matrix_vector_col_major_with_strides(64);
    benchmark_matrix_vector_col_major_with_strides(256);
    benchmark_matrix_vector_col_major_with_strides(512);
    benchmark_matrix_vector_col_major_with_strides(1024);

    benchmark_mm_naive_with_strides(64);
    benchmark_mm_naive_with_strides(256);
    benchmark_mm_naive_with_strides(512);
    benchmark_mm_naive_with_strides(1024);
}
