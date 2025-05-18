#pragma once
#include <string>
#include <chrono>

// Define a simple Order struct
struct Order {
    int orderId;
    std::string symbol;
    int quantity;
    double price;
    bool isBuy;
    std::chrono::high_resolution_clock::time_point enqueueTime;
    std::chrono::high_resolution_clock::time_point dequeueTime;
};
// OrderTypes.hpp
#pragma once
#include <string>
#include <chrono>

// Define a simple Order struct
struct Order {
    int orderId;
    std::string symbol;
    int quantity;
    double price;
    bool isBuy;
    std::chrono::high_resolution_clock::time_point enqueueTime;
    std::chrono::high_resolution_clock::time_point dequeueTime;
};