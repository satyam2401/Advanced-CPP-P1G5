#include "OrderBook.hpp"
#include <iostream>

// Constructor
template<typename PriceType, typename OrderIdType>
OrderBook<PriceType, OrderIdType>::OrderBook(std::size_t pool_size)
    : pool_(pool_size) {}

// add_order with symbol forwarding
template<typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::add_order(const OrderIdType& id,
                                                   const std::string& symbol,
                                                   PriceType price,
                                                   int quantity,
                                                   bool is_buy) {
    DeleterType deleter = [this](OrderType* ptr) {
        pool_.deallocate(ptr);
    };

    // placement new constructs Order using constructor args
    OrderPtr order(pool_.allocate(id, symbol, price, quantity, is_buy), deleter);
    if (!order) {
        std::cerr << "OrderBook memory pool exhausted\n";
        return;
    }

    if (is_buy) {
        auto iter = buy_orders_.emplace(price, std::move(order));
        id_lookup_[id] = iter;
    } else {
        auto iter = sell_orders_.emplace(price, std::move(order));
        id_lookup_[id] = iter;
    }
}

// get best bid
template<typename PriceType, typename OrderIdType>
typename OrderBook<PriceType, OrderIdType>::OrderType*
OrderBook<PriceType, OrderIdType>::get_best_bid() const {
    if (buy_orders_.empty()) return nullptr;
    return buy_orders_.begin()->second.get();
}

// get best ask
template<typename PriceType, typename OrderIdType>
typename OrderBook<PriceType, OrderIdType>::OrderType*
OrderBook<PriceType, OrderIdType>::get_best_ask() const {
    if (sell_orders_.empty()) return nullptr;
    return sell_orders_.begin()->second.get();
}

// remove filled/cancelled order
template<typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::remove_order(const OrderIdType& id) {
    auto it = id_lookup_.find(id);
    if (it == id_lookup_.end()) return;
    auto order_it = it->second;
    if (order_it->second->is_buy_order()) {
        buy_orders_.erase(order_it);
    } else {
        sell_orders_.erase(order_it);
    }
    id_lookup_.erase(it);
}

// print full book
template<typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::print_order_book() const {
    std::cout << "Buy Orders:" << std::endl;
    for (auto it = buy_orders_.rbegin(); it != buy_orders_.rend(); ++it)
        print_order(*it->second);
    std::cout << "Sell Orders:" << std::endl;
    for (auto& kv : sell_orders_)
        print_order(*kv.second);
}

// helper printer
template<typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::print_order(const OrderType& order) const {
    std::cout << "ID=" << order.get_id()
              << " Sym=" << order.get_symbol()
              << " Price=" << order.get_price()
              << " Qty=" << order.get_quantity()
              << " Side=" << (order.is_buy_order() ? "Buy" : "Sell")
              << std::endl;
}

// Explicit instantiation for common types
template class OrderBook<double,int>;
