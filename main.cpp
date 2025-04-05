#include <iostream>
#include "./test/MatrixOpsTest.cpp"
#include "./Phase 1/MatrixOps.h"

using namespace std;
int main() {
    std::cout<<"Matrix-Vector Multiplication (Row-Major) results:"<<endl;
    run_benchmarks_matrix_vector_multiplication();
    std::cout<<"\nMatrix-Vector Multiplication (Column-Major) results:"<<endl;
    run_benchmarks_matrix_vector_col_major_multiplication();
    std::cout<<"\nMatrix-Matrix Multiplication (Naive) results:"<<endl;
    run_benchmarks_mm_naive_multiplication();

    std::cout<<"\nCache Locality Benchmark for Column-Major Implementation:"<<endl;
    run_cache_locality_benchmarks();

    std::cout << "Running benchmarks for aligned column-major matrix-vector multiplication:\n";
    run_benchmarks_matrix_vector_col_major_aligned();

    return 0;
}
