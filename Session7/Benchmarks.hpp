#pragma once
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

// Utility class for timing benchmarks
class BenchmarkTimer {
public:
    BenchmarkTimer(const std::string& name) : name(name) {
        start = std::chrono::high_resolution_clock::now();
    }

    ~BenchmarkTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << name << ": " << duration << " microseconds\n";
    }

private:
    std::string name;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

// Class to collect benchmark results
class BenchmarkResults {
public:
    void addResult(const std::string& name, double durationMs) {
        results[name].push_back(durationMs);
    }

    void printResults() const {
        std::cout << "\n--- Benchmark Results ---\n";

        for (const auto& [name, durations] : results) {
            std::cout << "Benchmark: " << name << "\n";

            if (durations.empty()) {
                std::cout << "  No data\n";
                continue;
            }

            // Calculate statistics
            double sum = std::accumulate(durations.begin(), durations.end(), 0.0);
            double mean = sum / durations.size();

            double variance = 0.0;
            for (double d : durations) {
                variance += (d - mean) * (d - mean);
            }
            variance /= durations.size();
            double stdDev = std::sqrt(variance);

            auto minmax = std::minmax_element(durations.begin(), durations.end());

            // Print statistics
            std::cout << "  Runs: " << durations.size() << "\n";
            std::cout << "  Mean: " << mean << " ms\n";
            std::cout << "  Min: " << *minmax.first << " ms\n";
            std::cout << "  Max: " << *minmax.second << " ms\n";
            std::cout << "  StdDev: " << stdDev << " ms\n\n";
        }
    }

private:
    std::unordered_map<std::string, std::vector<double>> results;
};