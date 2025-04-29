// test_latency.cpp
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>

#include "MarketData.hpp"
#include "MarketDataFeedSimulator.h"
#include "OrderManager.hpp"
#include "OrderBook.hpp"
#include "MatchingEngine.hpp"
#include "TradeLogger.hpp"

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
    std::cout << "  Min   : " << min   << "\n"
              << "  Max   : " << max   << "\n"
              << "  Mean  : " << mean  << "\n"
              << "  StdDev: " << stddev<< "\n"
              << "  P99   : " << p99   << "\n";
}

int main() {
    using Clock = std::chrono::high_resolution_clock;

    // ——— Setup ———
    const int    numTicks = 100000;
    std::vector<long long> latencies;
    latencies.reserve(numTicks);

    // symbols for the market-data simulator
    std::vector<std::string> symbols = {"AAPL","GOOGL","MSFT","AMZN","TSLA"};
    MarketDataFeedSimulator simulator(symbols);

    // core HFT components
    OrderManager<double,int>   oms;
    OrderBook<double,int>      book;
    MatchingEngine<double,int> engine(book);
    TradeLogger<double,int>    logger(numTicks);

    // for randomizing buy/sell side and quantity
    std::mt19937                        rng(std::random_device{}());
    std::uniform_int_distribution<int> sideDist(0,1);
    std::uniform_int_distribution<int> qtyDist(1,100);

    // ——— Run the latency test ———
    for (int i = 0; i < numTicks; ++i) {
        // 1️⃣ generate a tick
        MarketData tick = simulator.generate_market_data();

        // 2️⃣ begin timing
        auto t0 = Clock::now();

        // 3️⃣ “handle” the tick by creating an order
        bool isBuy = (sideDist(rng)==0);
        int  qty   = qtyDist(rng);
        oms.create_order(i, tick.symbol, tick.bid_price, qty, isBuy);

        // 4️⃣ insert into the book
        auto orderPtr = oms.get_order(i);
        book.add_order(orderPtr->get_id(),
                       orderPtr->get_price(),
                       orderPtr->get_quantity(),
                       orderPtr->is_buy_order());

        // 5️⃣ run matching
        auto trades = engine.match_orders();
        for (auto &tr : trades) {
            logger.log_trade(tr);
        }

        // 6️⃣ stop timing & record
        auto t1 = Clock::now();
        latencies.push_back(
            std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()
        );
    }

    // ——— Report ———
    analyzeLatencies(latencies);
    logger.flush_to_file("trade_log.csv");

    return 0;
}
