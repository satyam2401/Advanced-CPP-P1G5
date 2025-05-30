#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <chrono>
#include "OrderTypes.hpp"
#include <shared_mutex>
#include <thread>
#include <unordered_map>
#include <cmath>

// Forward declarations
std::vector<Order> getCompletedOrders();

void runAnalytics() {
    // Get a copy of all completed orders
    auto completedOrders = getCompletedOrders();

    if (completedOrders.empty()) {
        std::cout << "No orders to analyze!\n";
        return;
    }

    std::cout << "\n--- Analytics Results ---\n";

    // Create a vector to store latencies (in microseconds)
    std::vector<double> latencies;
    latencies.reserve(completedOrders.size());

    // Calculate the processing latency for each order
    for (const auto& order : completedOrders) {
        double microsecs = std::chrono::duration_cast<std::chrono::microseconds>(
                order.dequeueTime - order.enqueueTime).count();
        latencies.push_back(microsecs);
    }

    // Compute the total latency
    double totalLatency = std::reduce(
            latencies.begin(),
            latencies.end(),
            0.0
    );

    // Calculate average latency
    double avgLatency = totalLatency / latencies.size();

    // Find min/max latencies
    auto minmax = std::minmax_element(
            latencies.begin(),
            latencies.end()
    );

    // Calculate standard deviation
    double varianceSum = std::transform_reduce(
            latencies.begin(),
            latencies.end(),
            0.0,
            std::plus<>(),
            [avgLatency](double x) { return (x - avgLatency) * (x - avgLatency); }
    );
    double stdDev = std::sqrt(varianceSum / latencies.size());

    // Calculate median
    std::vector<double> latenciesCopy = latencies;
    std::size_t n = latenciesCopy.size() / 2;
    std::nth_element(
            latenciesCopy.begin(),
            latenciesCopy.begin() + n,
            latenciesCopy.end()
    );
    double median = latenciesCopy[n];

    // Count orders by symbol
    std::unordered_map<std::string, int> symbolCounts;
    for (const auto& order : completedOrders) {
        symbolCounts[order.symbol]++;
    }

    // Print results
    std::cout << "Total Orders Processed: " << completedOrders.size() << "\n";
    std::cout << "Average Latency: " << avgLatency << " microseconds\n";
    std::cout << "Median Latency: " << median << " microseconds\n";
    std::cout << "Min Latency: " << *minmax.first << " microseconds\n";
    std::cout << "Max Latency: " << *minmax.second << " microseconds\n";
    std::cout << "Standard Deviation: " << stdDev << " microseconds\n";

    std::cout << "\nOrders by Symbol:\n";
    for (const auto& [symbol, count] : symbolCounts) {
        std::cout << symbol << ": " << count << " orders\n";
    }

    // Count buy orders
    int buyCount = std::count_if(
            completedOrders.begin(),
            completedOrders.end(),
            [](const Order& o) { return o.isBuy; }
    );

    double buyPercentage = 100.0 * buyCount / completedOrders.size();
    std::cout << "\nBuy Orders: " << buyCount
              << " (" << buyPercentage << "%)\n";
    std::cout << "Sell Orders: " << (completedOrders.size() - buyCount)
              << " (" << (100.0 - buyPercentage) << "%)\n";
}
