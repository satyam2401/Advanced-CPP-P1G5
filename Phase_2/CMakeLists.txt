cmake_minimum_required(VERSION 3.10)
project(MomentumClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O2")

# Add source files
add_executable(momentum_client
        hft_server.cpp
        hft_client.cpp
        unistd.h
        arpa/inet.h
        netinet/in.h
        )