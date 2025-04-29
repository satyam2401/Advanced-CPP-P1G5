#pragma once

#include <vector>
#include <fstream>
#include <string>

template<typename PriceType, typename OrderIdType>
struct Trade {
    OrderIdType buy_order_id;
    OrderIdType sell_order_id;
    PriceType price;
    int quantity;
};

template<typename PriceType, typename OrderIdType>
class TradeLogger {
public:
    using TradeType = Trade<PriceType, OrderIdType>;

    explicit TradeLogger(size_t reserve_size = 1000);

    void log_trade(const TradeType& trade);

    void flush_to_file(const std::string& filename);

private:
    std::vector<TradeType> trades_;
};