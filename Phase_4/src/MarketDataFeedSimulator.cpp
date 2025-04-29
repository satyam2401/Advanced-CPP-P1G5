#include "MarketDataFeedSimulator.h"
#include <random>
#include <chrono>
#include <iostream>
#include <thread>
#include <iomanip>

// Constructor: seed RNG and set price distribution
MarketDataFeedSimulator::MarketDataFeedSimulator(const std::vector<std::string>& symbols)
    : symbols_(symbols),
      rng_(std::random_device{}()),
      price_dist_(100.0, 500.0)
{}

// Generate one tick by calling the 3-arg MarketData ctor
MarketData MarketDataFeedSimulator::generate_market_data() {
    // 1) Pick a symbol at random
    const std::string& sym = symbols_[ rng_() % symbols_.size() ];

    // 2) Generate bid/ask around a random mid
    double mid = price_dist_(rng_);
    double bid = mid - 0.15 * mid * uniform_dist_(rng_);
    double ask = mid + 0.15 * mid * uniform_dist_(rng_);

    // 3) Construct and return the MarketData (auto‐stamps timestamp)
    return MarketData(sym, bid, ask);
}

// (Optional) Helper to print a tick
void print_market_data(const MarketData& md) {
    auto ts = std::chrono::duration_cast<std::chrono::seconds>(
                  md.timestamp.time_since_epoch()
              ).count();
    std::cout << std::fixed << std::setprecision(2)
              << "Symbol: " << md.symbol
              << ", Bid: "   << md.bid_price
              << ", Ask: "   << md.ask_price
              << ", Time: "  << ts
              << std::endl;
}

/*
int main(){
    std::vector<std::string> symbols = {"AAPL","GOOGL","MSFT","AMZN","TSLA"};
    MarketDataFeedSimulator simu(symbols);

    for(int i=0; i<10; ++i){
        auto md = simu.generate_market_data();
        print_market_data(md);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
*/
