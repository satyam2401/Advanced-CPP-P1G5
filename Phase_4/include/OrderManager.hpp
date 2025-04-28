//
// Created by Satyam Saurabh on 19/04/25.
//

#ifndef PHASE_3_ORDER_MANAGER_H
#define PHASE_3_ORDER_MANAGER_H

#include <map>
#include <memory>
#include <iostream>

enum class Side { Buy, Sell };
enum class OrderStatus { New, Filled, PartiallyFilled, Cancelled };

struct MyOrder {
    int id;
    Side side;
    double price;
    int quantity;
    int filled = 0;
    OrderStatus status = OrderStatus::New;

    MyOrder(int id, Side s, double p, int q);
};


class OrderManager {
    std::map<int, std::unique_ptr<MyOrder>> orders;
    int next_id = 1;

public:
    int place_order(Side side, double price, int qty);
    void cancel(int id);
    void handle_fill(int id, int filled_qty);
    void print_active_orders() const;
};

#endif //PHASE_3_ORDER_MANAGER_H
