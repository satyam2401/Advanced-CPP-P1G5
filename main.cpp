#include <iostream>
#include "./test/MatrixOpsTest.cpp"

using namespace std;
int main() {
    std::cout<<"Matrix-Vector Multiplication (Row-Major) results:"<<endl;
    run_benchmarks_matrix_vector_multiplication();
    return 0;
}
