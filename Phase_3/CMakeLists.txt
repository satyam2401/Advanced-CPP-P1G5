cmake_minimum_required(VERSION 3.29)
project(Phase_3)

set(CMAKE_CXX_STANDARD 20)

add_executable(Phase_3 main.cpp
        market_snapshot.cpp
        order_manager.cpp)
