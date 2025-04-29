#include "MarketDataFeedSimulator.h"
#include "MarketData.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>


MarketDataFeedSimulator::MarketDataFeedSimulator(const std::vector<std::string>& symbols):
    symbols_(symbols), rng_(std::random_device{}()),price_dist_(100.0, 500.0) {}

MarketData MarketDataFeedSimulator::generate_market_data(){
    MarketData md;
    md.symbol = symbols_[rng_() % symbols_.size()];
    double mid = price_dist_(rng_);
    md.bid_price = mid - 0.15 * mid * uniform_dist_(rng_);
    md.ask_price = mid + 0.15 * mid * uniform_dist_(rng_);
    md.timestamp = std::chrono::high_resolution_clock::now();;
    return md;
}


void print_market_data(const MarketData& md) {
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(md.timestamp.time_since_epoch()).count();
    std::cout << std::fixed << std::setprecision(2)
              << "Symbol: " << md.symbol
              << ", Bid Price: " << md.bid_price
              << ", Ask Price: " << md.ask_price
              << ", Timestamp: " << timestamp
              << std::endl;
}

// int main(){
//     std::vector<std::string> symbols = {"AAPL", "GOOGL", "MSFT", "AMZN", "TSLA"};
//     MarketDataFeedSimulator simulator(symbols);

//     const int num_iterations = 10;
//     for (int i = 0; i < num_iterations; ++i) {
//         MarketData md = simulator.generate_market_data();
//         print_market_data(md);
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
//     }

//     return 0;
// }

