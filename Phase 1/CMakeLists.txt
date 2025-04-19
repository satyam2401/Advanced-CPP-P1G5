cmake_minimum_required(VERSION 3.10)
project(MatrixBenchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O2")

# Add source files
add_executable(matrix_benchmark
        main.cpp
        Phase\ 1/MatrixOps.cpp
)