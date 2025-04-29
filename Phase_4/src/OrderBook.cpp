#include "OrderBook.hpp"
#include <iostream>

template <typename PriceType, typename OrderIdType>
OrderBook<PriceType, OrderIdType>::OrderBook() : memory_pool_(1024) {}


template <typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::add_order(const OrderIdType& id, PriceType price, int quantity, bool is_buy) {
    DeleterType deleter = [this](OrderType* ptr) {
        memory_pool_.deallocate(ptr);
    };

    OrderPtr order(memory_pool_.allocate(), deleter);

    order->set(id, price, quantity, is_buy);

    if (is_buy) {
        auto iter = buy_orders_.emplace(price, std::move(order));
        id_lookup_[id] = iter;
    } else {
        auto iter = sell_orders_.emplace(price, std::move(order));
        id_lookup_[id] = iter;
    }
}

template <typename PriceType, typename OrderIdType>
typename OrderBook<PriceType, OrderIdType>::OrderType*
OrderBook<PriceType, OrderIdType>::get_best_bid() const {
    if (buy_orders_.empty()) {
        return nullptr;
    }
    return buy_orders_.begin()->second.get();
}

template <typename PriceType, typename OrderIdType>
typename OrderBook<PriceType, OrderIdType>::OrderType*
OrderBook<PriceType, OrderIdType>::get_best_ask() const {
    if (sell_orders_.empty()) {
        return nullptr;
    }
    return sell_orders_.begin()->second.get();
}

template <typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::remove_order(const OrderIdType& id) {
    auto it = id_lookup_.find(id);
    if (it != id_lookup_.end()) {
        auto order_it = it->second;
        if (order_it->second->is_buy_order()) {
            buy_orders_.erase(order_it);
        } else {
            sell_orders_.erase(order_it);
        }
        id_lookup_.erase(it);
    }
}

template <typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::print_order_book() const {
    std::cout << "Buy Orders:" << std::endl;
    for (auto it = buy_orders_.rbegin(); it != buy_orders_.rend(); ++it) {
        print_order(*it->second);
    }
    std::cout << "Sell Orders:" << std::endl;
    for (auto& pair : sell_orders_) {
        print_order(*pair.second);
    }
}

template <typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::print_order(const OrderType& order) const {
    std::cout << "Order ID: " << order.get_id()
              << ", Price: " << order.get_price()
              << ", Quantity: " << order.get_quantity()
              << ", Type: " << (order.is_buy_order() ? "Buy" : "Sell") << std::endl;
}

// int main() {
//     OrderBook<double, int> ob;

//     ob.add_order(1, 100.5, 10, true);
//     ob.add_order(2, 101.0, 5, true);
//     ob.add_order(3, 102.0, 20, false);
//     ob.add_order(4, 103.5, 15, false);

//     ob.print_order_book();

//     return 0;
// }

template class OrderBook<double, int>;