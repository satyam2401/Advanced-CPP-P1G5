#pragma once

#include <vector>
#include "MatchingEngine.hpp"  // for TradeType definition
#include <fstream>
#include <string>

/**
 * @brief Logs completed trades to an in-memory buffer and flushes to CSV.
 */
template<typename PriceType, typename OrderIdType>
class TradeLogger {
public:
    // Use the Trade struct from MatchingEngine
    using TradeType = typename MatchingEngine<PriceType, OrderIdType>::TradeType;

    /**
     * @param reserve_size Number of trades to reserve space for
     */
    explicit TradeLogger(size_t reserve_size = 1000);

    /**
     * @brief Add a completed trade to the log buffer.
     */
    void log_trade(const TradeType& trade);

    /**
     * @brief Write all buffered trades to a CSV file.
     */
    void flush_to_file(const std::string& filename);

private:
    std::vector<TradeType> trades_;
};
