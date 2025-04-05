Team Members (Part 1 group 5):
- Sherry Lou
- Xitaaz Rampersad
- Joe Cooney
- Satyam Saurabh


Build Instructions:
- Make sure you're in the root directory of the project.

Step 1: Generate build files using CMake
> cmake -S . -B cmake-build-debug

Step 2: Build the project
> cmake --build cmake-build-debug

Step 3: Run the executable
> ./cmake-build-debug/Advanced_CPP_P1G5
>
1. Pointers are a data type which stores an address while references are the memory address. pointers are more useful for memory allocation, while references are more useful for function arguments.
2. Row-major is faster. In the cache line, a full row is cached and used in row-major, while in column major, only one value per cached row is used. (provide specific example)
3. L1 is closest to the CPU, and L2. then L3 are further away. The CPU tries to get data from the L1 cache before the other two, which, if found, is fastest because the L1 cache is phycially closest to the CPU. We exploited this in our row-major implementations. (PART 2)
4. Memory alignment refers to the spatial locality of data in memory. For example, in row-major calculations, all data in one row is consecutive in memory.
5. Compiler optimizations did not significantly improve performance for the naive matrix-matrix and column-major matrix-vector multiplication implementations. The level of optimization only marginally affected these implementations' performance, but took substantial time to code. The main drawback of these optimizations is an increased complexity of code that takes more human resources to maintain, debeg, and create. However, in a high-frequency trading context, these costs may be minimal compared to the increased profit that optimized code could generate.
6. (havent done this yet)
7. we use different operating systems in our group, and this made collaboration on the profiling aspect more difficult. We went with a mac hardware approach for the profiling, but broke up the analysis tasks by whoever did that part of the code, then regrouped. 
