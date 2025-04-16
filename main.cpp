#include <iostream>
#include "./test/MatrixOpsTest.cpp"
#include "./Phase 1/MatrixOps.h"

using namespace std;
int main() {
    // std::cout<<"Matrix-Vector Multiplication (Row-Major) results:"<<endl;
    // run_benchmarks_matrix_vector_multiplication();
    // std::cout<<"\nMatrix-Vector Multiplication (Column-Major) results:"<<endl;
    // run_benchmarks_matrix_vector_col_major_multiplication();
    // std::cout<<"\nMatrix-Matrix Multiplication (Naive) results:"<<endl;
    // run_benchmarks_mm_naive_multiplication();
    std::cout<<"\nMatrix-Matrix Multiplication (Transposed B) results:"<<endl;
    //run_benchmarks_mm_transpose();

    std::cout<<"\nCache Locality Benchmarks:"<<endl;
    run_cache_locality_benchmarks();

    // std::cout << "Running benchmarks for aligned column-major matrix-vector multiplication:\n";
    // run_benchmarks_matrix_vector_col_major_aligned();
    // run_benchmarks_mm_naive_aligned();
    //run_benchmarks_mm_transpose_aligned();

    // std::cout << "Running benchmarks for inline function:\n";
    // run_benchmarks_matrix_vector_col_major_multiplication_inline();
    // run_benchmarks_mm_naive_inline();
    //run_benchmarks_mm_transpose_inline();
    return 0;
}
