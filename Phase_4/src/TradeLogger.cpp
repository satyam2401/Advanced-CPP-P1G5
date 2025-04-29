#include "TradeLogger.h"
#include <iostream>

template<typename PriceType, typename OrderIdType>
TradeLogger<PriceType, OrderIdType>::TradeLogger(size_t reserve_size) {
    trades_.reserve(reserve_size);
}

template<typename PriceType, typename OrderIdType>
void TradeLogger<PriceType, OrderIdType>::log_trade(const TradeType& trade) {
    trades_.push_back(trade);
}

template<typename PriceType, typename OrderIdType>
void TradeLogger<PriceType, OrderIdType>::flush_to_file(const std::string& filename) {
    std::ofstream file(filename, std::ios::out);

    if (!file.is_open()) {
        std::cerr << "Failed to open trade log file: " << filename << std::endl;
        return;
    }

    for (const auto& trade : trades_) {
        file << trade.buy_order_id << ","
             << trade.sell_order_id << ","
             << trade.price << ","
             << trade.quantity << "\n";
    }

    file.close();
    std::cout << "Trade log written to " << filename << std::endl;
}

template class TradeLogger<double, int>;