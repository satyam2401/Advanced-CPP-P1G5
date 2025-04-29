#pragma once

#include <iostream>
#include <chrono>

struct alignas(64) MarketData{
    std::string symbol;
    double bid_price;
    double ask_price;
    std::chrono::high_resolution_clock::time_point timestamp;

    MarketData(const std::string& symbol, double bid_price, double ask_price)
        : symbol(symbol), bid_price(bid_price), ask_price(ask_price), 
          timestamp(std::chrono::high_resolution_clock::now()) {}


};