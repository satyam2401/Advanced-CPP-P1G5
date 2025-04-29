#pragma once

#include "Order.hpp"
#include "MemoryPool.h"
#include <map>
#include <memory>
#include <iostream>
#include <functional>
#include <unordered_map>

template<typename PriceType, typename OrderIdType>
class OrderBook {
public:
    using OrderType = Order<PriceType, OrderIdType>;
    using DeleterType = std::function<void(OrderType*)>;
    using OrderPtr = std::unique_ptr<OrderType, DeleterType>;

    OrderBook();

    void add_order(const OrderIdType& id, PriceType price, int quantity, bool is_buy);

    void print_order_book() const;

    OrderType* get_best_bid() const;
    OrderType* get_best_ask() const;

    void remove_order(const OrderIdType& id);

private:
    void print_order(const OrderType& order) const;

    MemoryPool<OrderType> memory_pool_;
    std::multimap<PriceType, OrderPtr, std::greater<PriceType>> buy_orders_;
    std::multimap<PriceType, OrderPtr> sell_orders_;
    std::unordered_map<OrderIdType, typename std::multimap<PriceType, OrderPtr>::iterator> id_lookup_;
};