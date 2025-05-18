cmake_minimum_required(VERSION 3.15)
project(TradingSystem VERSION 1.0)

# C++20 is required for std::latch, std::barrier, and std::counting_semaphore
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find threading package
find_package(Threads REQUIRED)

# Add compile options
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Create the main trading system executable
add_executable(trading_system
        main.cpp
        OrderProcessing.cpp
        Analytics.cpp
)

# Link with threading library
target_link_libraries(trading_system PRIVATE Threads::Threads)

# Create the memory ordering benchmark executable
add_executable(memory_benchmark
        MemoryOrderingBenchmark.cpp
)

# Link with threading library
target_link_libraries(memory_benchmark PRIVATE Threads::Threads)

# Installation
install(TARGETS trading_system memory_benchmark
        RUNTIME DESTINATION bin
)

# Testing
enable_testing()

# Add a simple test to make sure the programs compile and run
add_test(NAME TradingSystemRunTest COMMAND trading_system --test-mode)
add_test(NAME MemoryBenchmarkRunTest COMMAND memory_benchmark --test-mode)

# Print status
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")