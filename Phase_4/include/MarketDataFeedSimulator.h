#pragma once

#include <vector>
#include <string>
#include <random>
#include "MarketData.hpp"

/**
 * @brief Simulates a market‐data feed by generating aligned MarketData ticks.
 */
class MarketDataFeedSimulator {
public:
    /**
     * @param symbols List of instrument symbols to cycle through
     */
    explicit MarketDataFeedSimulator(const std::vector<std::string>& symbols);

    /**
     * @brief Generate one MarketData tick (symbol, bid/ask, timestamp)
     */
    MarketData generate_market_data();

private:
    std::vector<std::string>                    symbols_;
    std::mt19937                                 rng_;
    std::uniform_real_distribution<>             price_dist_;
    std::uniform_real_distribution<>             uniform_dist_{0.0, 1.0};
};
