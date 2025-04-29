#pragma once

#include <string>
#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include "Order.hpp"
#include "MemoryPool.h"

/**
 * @brief A templated limit order book with fast in-place Order allocation.
 */
template<typename PriceType, typename OrderIdType>
class OrderBook {
public:
    using OrderType   = Order<PriceType, OrderIdType>;
    using DeleterType = std::function<void(OrderType*)>;
    using OrderPtr    = std::unique_ptr<OrderType, DeleterType>;

    /**
     * @param pool_size Number of Order slots in the memory pool
     */
    explicit OrderBook(std::size_t pool_size = 1024);

    /**
     * @brief Insert a new order into the book.
     * @param id      Unique order identifier
     * @param symbol  Instrument symbol
     * @param price   Limit price
     * @param quantity Order quantity
     * @param is_buy  Side flag
     */
    void add_order(const OrderIdType& id,
                   const std::string& symbol,
                   PriceType price,
                   int quantity,
                   bool is_buy);

    void print_order_book() const;
    OrderType* get_best_bid() const;
    OrderType* get_best_ask() const;
    void remove_order(const OrderIdType& id);

private:
    void print_order(const OrderType& order) const;

    MemoryPool<OrderType> pool_;
    std::multimap<PriceType, OrderPtr, std::greater<PriceType>> buy_orders_;
    std::multimap<PriceType, OrderPtr>                          sell_orders_;
    std::unordered_map<OrderIdType,
        typename decltype(buy_orders_)::iterator>               id_lookup_;
};