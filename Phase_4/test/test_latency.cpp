#include <iostream>
#include<vector>
#include<chrono>
#include<numeric>
#include"../include/MarketData.hpp"
#include"../include/MatchingEngine.hpp"

void analyzeLatencies(const std::vector<long long>& latencies) {
    if (latencies.empty()) return;

    auto min = *std::min_element(latencies.begin(), latencies.end());
    auto max = *std::max_element(latencies.begin(), latencies.end());
    double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
    double variance = 0.0;
    for (auto l : latencies) variance += (l - mean) * (l - mean);
    double stddev = std::sqrt(variance / latencies.size());
    long long p99 = latencies[static_cast<int>(latencies.size() * 0.99)];

    std::cout << "Tick-to-Trade Latency (nanoseconds):\n";
    std::cout << "Min: " << min << "\nMax: " << max << "\nMean: " << mean
              << "\nStdDev: " << stddev << "\nP99: " << p99 << "\n";
}

using Clock = std::chrono::high_resolution_clock;
auto start = Clock::now();

// Simulate tick reception
marketDataHandler.handleTick(tick);

// Match & trade generation
matchingEngine.matchOrder(order);

auto end = Clock::now();
auto latency = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();