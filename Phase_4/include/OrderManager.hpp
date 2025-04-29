#pragma once
#include "Order.hpp"
#include <unordered_map>
#include <memory>
#include <iostream>

template<typename PriceType, typename OrderIdType>
class OrderManager{
public:
    static_assert(std::is_integral<OrderIdType>::value, "Order ID must be an integer");

    using OrderType = Order<PriceType, OrderIdType>;
    using OrderPtr  = std::shared_ptr<OrderType>;

    OrderManager() = default;

    void create_order(const OrderIdType& id, const std::string& symbol, PriceType price, int quantity, bool is_buy);
    void fill_order(OrderIdType id, int quantity);
    void cancel_order(OrderIdType id);
    void print_all_orders() const;

    OrderPtr get_order(const OrderIdType& id) const;

private:
    std::unordered_map<OrderIdType, OrderPtr> orders_;
};
