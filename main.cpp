#include <iostream>
#include "./test/MatrixOpsTest.cpp"
#include "./Phase 1/MatrixOps.h"

using namespace std;
int main() {
    std::cout<<"Matrix-Vector Multiplication (Row-Major) results:"<<endl;
    run_benchmarks_matrix_vector_multiplication();
    std::cout<<"Matrix-Matrix Multiplication (Naive) results:"<<endl;
    run_benchmarks_mm_naive_multiplication();
    return 0;
}
