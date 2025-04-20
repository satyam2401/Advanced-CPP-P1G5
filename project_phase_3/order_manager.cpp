#include "order_manager.h"
#include <iostream>
#include <memory>
#include <string>
#include <map>

int OrderManager::place_order(Side side, double price, int qty){
    int id = next_order_id ++;
    orders[id] = std::make_unique<MyOrder>(id, side,price,qty);
    std::cout << "Placed order ID "<< id << " (" << to_string(side) <<") at" << price << " x " << qty << "\n";
    return id; 
}

void OrderManager::cancel(int id){
    if (orders.count(id)){
        orders[id] -> status = OrderStatus::Cancelled;
        std::cout << "Cancelled order ID " << id << std::endl;
        orders.erase(id);
    }
}

void OrderManager::handle_fill(int id, int filled_qty){
    if (!orders.count(id)) return;

    MyOrder* order = orders[id].get();
    order -> filled += filled_qty;

    if (order -> filled >= order -> quantity){
        order -> status = OrderStatus::Filled;
        std::cout << "Order ID " << id << "filled completely.\n";
        orders.erase(id);
    } else {
        order -> status = OrderStatus::ParticallyFilled;
        std::cout << "Order ID" << id << "partial fill: " << order -> filled << "/" << order -> quantity << std::endl;
    }

}

void OrderManager::print_active_orders() const{
    std::cout << "\n Active Orders: \n";
    for (const auto& [id, order]: orders){
        std::cout << " ID "<< id << " | "<< to_string(order-> side) << " | " << order -> price 
        << " x " << order -> quantity << " (filled: " << order ->filled << ")\n";
    }

    if (orders.empty()) std::cout << " (none)\n";
}



// int main() {
//     OrderManager manager;

//     int id1 = manager.place_order(Side::Buy, 100.25, 200);
//     int id2 = manager.place_order(Side::Sell, 100.35, 150);

//     manager.print_active_orders();

//     manager.handle_fill(id1, 50);
//     manager.handle_fill(id1, 150); // full fill
//     manager.handle_fill(id2, 100);
//     manager.cancel(id2);           // cancel remaining

//     manager.print_active_orders();
// }