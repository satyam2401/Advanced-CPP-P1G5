# ifndef MarketDataFeedSimulator_H
# define MarketDataFeedSimulator_H


#include <vector>
#include <string>
#include <chrono>
#include <random>
#include "MarketData.h"

struct alignas(64) MarketData{
    std::string symbol;
    double bid_price;
    double ask_price;
    std::chrono::high_resolution_clock::time_point timestamp;
};

class MarketDataFeedSimulator{
private:
    std::vector<std::string> symbols_;
    std::mt19937 rng_;
    std::uniform_real_distribution<> price_dist_;
    std::uniform_real_distribution<> uniform_dist_{0.0,1.0};

public:
    MarketDataFeedSimulator(const std::vector<std::string>& symbols);
    MarketData generate_market_data();
};

#endif