#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "OrderBook.hpp"
#include "Timer.hpp"

int main() {
    // 1) Prepare latency vector and book
    std::vector<long long> latencies;
    const int num_ticks = 10000;
    latencies.reserve(num_ticks);

    OrderBook<double,int> book(1024);

    // 2) Simulate ticks & measure tick→trade latency
    for (int i = 0; i < num_ticks; ++i) {
        Timer timer;
        timer.start();

        std::string symbol = "AAPL";
        double price       = 150.0 + (i % 5);
        int    qty         = 100;
        bool   isBuy       = (i % 2 == 0);

        book.add_order(i, symbol, price, qty, isBuy);

        latencies.push_back(timer.stop());
    }

    // 3) Print simple stats
    auto min = *std::min_element(latencies.begin(), latencies.end());
    auto max = *std::max_element(latencies.begin(), latencies.end());
    double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();

    std::cout << "Tick-to-Trade Latency (nanoseconds):\n"
              << "  Min : " << min  << "\n"
              << "  Max : " << max  << "\n"
              << "  Mean: " << mean << "\n";
    return 0;
}
