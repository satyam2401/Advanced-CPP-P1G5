//
// Created by Satyam Saurabh on 19/04/25.
//

#include "order_manager.h"

MyOrder::MyOrder(int id, Side s, double p, int q)
        : id(id), side(s), price(p), quantity(q) {}

int OrderManager::place_order(Side side, double price, int qty) {
    int id = next_id++;
    orders[id] = std::make_unique<MyOrder>(id, side, price, qty);
    return id;
}

void OrderManager::cancel(int id) {
    auto it = orders.find(id);
    if (it != orders.end()) {
        it->second->status = OrderStatus::Cancelled;
    }
}

void OrderManager::handle_fill(int id, int filled_qty) {
    auto it = orders.find(id);
    if (it != orders.end()) {
        MyOrder* order = it->second.get();
        order->filled += filled_qty;
        if (order->filled >= order->quantity) {
            order->status = OrderStatus::Filled;
        } else {
            order->status = OrderStatus::PartiallyFilled;
        }
    }
}

void OrderManager::print_active_orders() const {
    for (const auto& [id, order] : orders) {
        if (order->status == OrderStatus::New || order->status == OrderStatus::PartiallyFilled) {
            std::cout << "Order " << id << " | Side: " << (order->side == Side::Buy ? "Buy" : "Sell")
                      << " | Price: " << order->price << " | Qty: " << order->quantity
                      << " | Filled: " << order->filled << "\n";
        }
    }
}

