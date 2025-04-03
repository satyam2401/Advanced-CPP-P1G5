#include <iostream>
#include "./test/MatrixOpsTest.cpp"

using namespace std;
int main() {
    std::cout<<"Matrix-Vector Multiplication (Row-Major) results:"<<endl;
    run_benchmarks_matrix_vector_multiplication();

    std::cout<<"Matrix-Matrixtransposed Multiplication (Row-MAjor) results:"<<endl;
    run_benchmarks_matrix_matrixtranposed_multiplication();

    return 0;
}
