#pragma once

#include "OrderBook.h"
#include <vector>

template <typename PriceType, typename OrderIdType>
struct Trade
{
    OrderIdType buy_order_id;
    OrderIdType sell_order_id;
    PriceType price;
    int quantity;
};

template <typename PriceType, typename OrderIdType>
class MatchingEngine{
public: 
    using OrderBookType = OrderBook<PriceType, OrderIdType>;
    using TradeType = Trade<PriceType, OrderIdType>;

    MatchingEngine(OrderBookType& order_book);

    std::vector<TradeType> match_orders();
private:
    OrderBookType& order_book_;
};