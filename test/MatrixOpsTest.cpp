#include <iostream>
#include <chrono>
#include <cmath>
#include "../Phase 1/MatrixOps.h"

#define ALIGNMENT 64

void benchmark_matrix_vector_multiplication(int rows, int cols, int runs = 25) {
    std::cout << "\n[Benchmark] Size: " << rows << "x" << cols << " | Runs: " << runs << "\n";

    // Allocate memory
    auto* matrix = new double[rows * cols];
    auto* vector = new double[cols];
    auto* result_baseline = new double[rows];
    auto* result_optimised = new double[rows];

    // Fill matrix and vector with dummy data
    for (int i = 0; i < rows * cols; ++i) matrix[i] = static_cast<double>((i % 10) + 1);
    for (int i = 0; i < cols; ++i) vector[i] = static_cast<double>((i % 5) + 1);

    double times_baseline[25];
    double times_optimised[25];
    double total_baseline = 0.0;
    double total_optimised = 0.0;

    // Benchmark baseline
    for (int r = 0; r < runs; ++r) {
        auto start = std::chrono::high_resolution_clock::now();
        MatrixOps::multiply_mv_row_major(matrix, rows, cols, vector, result_baseline);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> duration = end - start;
        times_baseline[r] = duration.count();
        total_baseline += times_baseline[r];
    }

    // Benchmark optimised
    for (int r = 0; r < runs; ++r) {
        auto start = std::chrono::high_resolution_clock::now();
        MatrixOps::multiply_mv_row_major_optimised(matrix, rows, cols, vector, result_optimised);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> duration = end - start;
        times_optimised[r] = duration.count();
        total_optimised += times_optimised[r];
    }

    // Compute averages
    double avg_baseline = total_baseline / runs;
    double avg_optimised = total_optimised / runs;

    // Compute standard deviations
    double var_baseline = 0.0, var_optimised = 0.0;
    for (int r = 0; r < runs; ++r) {
        var_baseline += std::pow(times_baseline[r] - avg_baseline, 2);
        var_optimised += std::pow(times_optimised[r] - avg_optimised, 2);
    }

    var_baseline /= runs;
    var_optimised /= runs;

    double stddev_baseline = std::sqrt(var_baseline);
    double stddev_optimised = std::sqrt(var_optimised);

    std::cout << "[Baseline]   Avg Time: " << avg_baseline << " us | Std Dev: " << stddev_baseline << " us\n";
    std::cout << "[Optimised]  Avg Time: " << avg_optimised  << " us | Std Dev: " << stddev_optimised  << " us\n";

    delete[] matrix;
    delete[] vector;
    delete[] result_baseline;
    delete[] result_optimised;
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

void benchmark_matrix_vector_col_major_multiplication_inline(int rows, int cols, int runs = 25) {
    std::cout << "\n[Inline] Column-Major MV | Size: " << rows << "x" << cols << " | Runs: " << runs << "\n";

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
        MatrixOps::multiply_mv_col_major_inline(matrix, rows, cols, vector, result);
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

void run_benchmarks_matrix_vector_col_major_multiplication_inline() {
    benchmark_matrix_vector_col_major_multiplication_inline(10, 10);       // Small
    benchmark_matrix_vector_col_major_multiplication_inline(500, 500);     // Medium
    benchmark_matrix_vector_col_major_multiplication_inline(1000, 1000);   // Large
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

void* aligned_alloc(size_t alignment, size_t size) {
    void* ptr = nullptr;

#ifdef _WIN32
    // Windows-specific aligned allocation
    ptr = _aligned_malloc(size, alignment);
#else
    if (posix_memalign(&ptr, alignment, size) != 0) {
        ptr = nullptr;
    }
#endif

    if (!ptr) {
        throw std::bad_alloc();
    }

    return ptr;
}

// Aligned memory deallocation
void aligned_free(void* ptr) {
#ifdef _WIN32
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

// Aligned smart pointer for automatic memory management
template <typename T>
struct AlignedDeleter {
    void operator()(T* ptr) const {
        aligned_free(ptr);
    }
};

template <typename T>
using AlignedUniquePtr = std::unique_ptr<T, AlignedDeleter<T>>;

template <typename T>
AlignedUniquePtr<T> make_aligned_unique(size_t size) {
    return AlignedUniquePtr<T>(static_cast<T*>(aligned_alloc(ALIGNMENT, size * sizeof(T))));
}

void multiply_mv_col_major_aligned(const double* matrix, int rows, int cols,
                                  const double* vector, double* result) {
    if (!matrix || !vector || !result) {
        std::cerr << "[ERROR] Null pointer passed to multiply_mv_col_major_aligned.\n";
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
}


void benchmark_matrix_vector_col_major_aligned(int rows, int cols, int runs = 25) {
    std::cout << "\n[Benchmark] Aligned Column-Major MV | Size: " << rows << "x" << cols
              << " | Alignment: " << ALIGNMENT << " bytes | Runs: " << runs << "\n";

    // Allocate aligned memory
    auto matrix = make_aligned_unique<double>(rows * cols);
    auto vector = make_aligned_unique<double>(cols);
    auto result = make_aligned_unique<double>(rows);

    // Fill matrix and vector with dummy data
    for (int col = 0; col < cols; ++col) {
        for (int row = 0; row < rows; ++row) {
            matrix.get()[col * rows + row] = static_cast<double>((col * rows + row) % 10 + 1);
        }
    }

    for (int i = 0; i < cols; ++i) {
        vector.get()[i] = static_cast<double>((i % 5) + 1);
    }

    // Store timings
    double total_time = 0.0;
    double times[25];

    for (int r = 0; r < runs; ++r) {
        auto start = std::chrono::high_resolution_clock::now();

        multiply_mv_col_major_aligned(matrix.get(), rows, cols, vector.get(), result.get());

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

    std::cout << "Matrix alignment: "
              << (reinterpret_cast<uintptr_t>(matrix.get()) % ALIGNMENT == 0 ? "Aligned" : "Not aligned") << "\n";
    std::cout << "Vector alignment: "
              << (reinterpret_cast<uintptr_t>(vector.get()) % ALIGNMENT == 0 ? "Aligned" : "Not aligned") << "\n";
    std::cout << "Result alignment: "
              << (reinterpret_cast<uintptr_t>(result.get()) % ALIGNMENT == 0 ? "Aligned" : "Not aligned") << "\n";
}

void run_benchmarks_matrix_vector_col_major_aligned() {
    benchmark_matrix_vector_col_major_aligned(10, 10);       // Small
    benchmark_matrix_vector_col_major_aligned(500, 500);     // Medium
    benchmark_matrix_vector_col_major_aligned(1000, 1000);   // Large
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

void multiply_mm_naive_aligned(const double* matrixA, const double* matrixB, 
                                int rowsA, int colsA, int rowsB, int colsB, double* result) {
    if (!matrixA || !matrixB || !result) {
    std::cerr << "[ERROR] Null pointer passed to multiply_mm_naive_aligned.\n";
    return;
    }

    if (rowsA <= 0 || colsA <= 0 || rowsB<=0 || colsB<=0 || colsA!=rowsB) {
    std::cerr << "[ERROR] Invalid matrix dimensions.\n";
    return;
    }

    // Initialize result array with zeros
    for (int rowA = 0; rowA < rowsA; ++rowA) {
        for (int colB = 0; colB < colsB; ++colB) {
            result[colsB * (rowA) + (colB)] = 0.0;
        }

    }

    for (int rowA = 0; rowA < rowsA; ++rowA){
        for (int colB = 0; colB < colsB; ++colB){
            double dotResult = 0.0;
            for (int colA = 0; colA < colsA; ++colA){
                dotResult += matrixA[colsA * (rowA) + (colA)] * matrixB[colsB * (colA) + (colB)];
            }
            result[colsB * (rowA) + (colB)] = dotResult;
        }
    }
}


void benchmark_mm_naive_aligned(int rowsA, int colsA, int rowsB, int colsB, int runs = 25) {
    std::cout << "\n[Benchmark] Aligned MM naive | Size: " << rowsA << "x" << colsB << ", " << rowsB << "x" << colsB
    << " | Alignment: " << ALIGNMENT << " bytes | Runs: " << runs << "\n";

    // Allocate aligned memory
    auto matrixA = make_aligned_unique<double>(rowsA * colsA);
    auto matrixB = make_aligned_unique<double>(rowsB * colsB);
    auto result = make_aligned_unique<double>(rowsA * colsB);

    // Fill matrix and vector with dummy data
    for (int rowA = 0; rowA < rowsA; ++rowA) {
        for (int colA = 0; colA < colsA; ++colA) {
            matrixA.get()[rowA * colsA + colA] = 1.0;
        }
    }

    // Fill matrix B with dummy data in row-major
    for (int rowB = 0; rowB < rowsB; ++rowB) {
        for (int colB = 0; colB < colsB; ++colB) {
            matrixB.get()[rowB * colsB + colB] = 1.0;
        }
    }


    // Store timings
    double total_time = 0.0;
    double times[25];

    for (int r = 0; r < runs; ++r) {
    auto start = std::chrono::high_resolution_clock::now();

    multiply_mm_naive_aligned(matrixA.get(), matrixB.get(), rowsA, colsA, rowsB, colsB, result.get());

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

    std::cout << "MatrixA alignment: "
    << (reinterpret_cast<uintptr_t>(matrixA.get()) % ALIGNMENT == 0 ? "Aligned" : "Not aligned") << "\n";
    std::cout << "MatrixB alignment: "
    << (reinterpret_cast<uintptr_t>(matrixB.get()) % ALIGNMENT == 0 ? "Aligned" : "Not aligned") << "\n";
    std::cout << "Result alignment: "
    << (reinterpret_cast<uintptr_t>(result.get()) % ALIGNMENT == 0 ? "Aligned" : "Not aligned") << "\n";
}

void benchmark_mm_naive_inline(int rowsA, int colsA, int rowsB, int colsB, int runs = 25) {
    std::cout << "\n[Inline] Column-Major MV | Size: " << rowsA << "x" << colsA << ", " << rowsB << "x" << colsB
    << " | Runs: " << runs << "\n";

    auto* matrixA = new double[rowsA * colsA];
    auto* matrixB = new double[rowsB * colsB];
    auto* result = new double[rowsA * colsB];

    // Fill matrices with dummy data

    for (int colA = 0; colA < colsA; ++colA) {
        for (int rowA = 0; rowA < rowsA; ++rowA) {
            matrixA[colA * rowsA + rowA] = static_cast<double>(1);
        }
    }

    for (int colB = 0; colB < colsB; ++colB) {
        for (int rowB = 0; rowB < rowsB; ++rowB) {
            matrixB[colB * rowsB + rowB] = static_cast<double>(1);
        }
    }

    // Store timings
    double total_time = 0.0;
    double times[25];

    for (int r = 0; r < runs; ++r) {
        auto start = std::chrono::high_resolution_clock::now();
        MatrixOps::multiply_mm_naive_inline(matrixA, rowsA, colsB, matrixB, rowsB, colsB, result);
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

void run_benchmarks_mm_naive_aligned() {
    benchmark_mm_naive_aligned(10, 10, 10, 10);       // Small
    benchmark_mm_naive_aligned(500, 500, 500, 500);     // Medium
    benchmark_mm_naive_aligned(1000, 1000, 1000, 1000);   // Large
}

void run_benchmarks_mm_naive_inline() {
    benchmark_mm_naive_inline(10, 10, 10, 10);
    benchmark_mm_naive_inline(500, 500, 500, 500);
    benchmark_mm_naive_inline(1000, 1000, 1000, 1000);
}


void run_cache_locality_benchmarks() {
    // benchmark_matrix_vector_col_major_with_strides(64);
    // benchmark_matrix_vector_col_major_with_strides(256);
    // benchmark_matrix_vector_col_major_with_strides(512);
    // benchmark_matrix_vector_col_major_with_strides(1024);

    // benchmark_mm_naive_with_strides(64);
    // benchmark_mm_naive_with_strides(256);
    // benchmark_mm_naive_with_strides(512);
    // benchmark_mm_naive_with_strides(1024);
}
