#include "OrderManager.h"

template<typename PriceType, typename OrderIdType>
void OrderManager<PriceType, OrderIdType>::create_order(const OrderIdType& id, const std::string& symbol, PriceType price, int quantity, bool is_buy) {
    auto order = std::make_shared<OrderType>(id, symbol, price, quantity, is_buy);
    orders_[id] = order;
}

template<typename PriceType, typename OrderIdType>
void OrderManager<PriceType, OrderIdType>::fill_order(OrderIdType id, int fill_quantity) {
    auto it = orders_.find(id);
    if (it != orders_.end()) {
        it->second->update_filled(fill_quantity);
    }
}

template<typename PriceType, typename OrderIdType>
void OrderManager<PriceType, OrderIdType>::cancel_order(OrderIdType id) {
    auto it = orders_.find(id);
    if (it != orders_.end()) {
        it->second->cancel();
    }
}

template<typename PriceType, typename OrderIdType>
void OrderManager<PriceType, OrderIdType>::print_all_orders() const {
    for (const auto& [id, order] : orders_) {
        std::cout << "Order ID: " << order->get_id()
                  << ", Price: " << order->get_price()
                  << ", Quantity: " << order->get_quantity()
                  << ", Filled: " << order->get_filled_quantity()
                  << ", Status: ";

        switch (order->get_status()) {
            case OrderStatus::New: std::cout << "New"; break;
            case OrderStatus::PartiallyFilled: std::cout << "Partially Filled"; break;
            case OrderStatus::Filled: std::cout << "Filled"; break;
            case OrderStatus::Cancelled: std::cout << "Cancelled"; break;
        }

        std::cout << ", Side: " << (order->is_buy_order() ? "Buy" : "Sell") << std::endl;
    }
}

template<typename PriceType, typename OrderIdType>
typename OrderManager<PriceType, OrderIdType>::OrderPtr
OrderManager<PriceType, OrderIdType>::get_order(const OrderIdType& id) const {
    auto it = orders_.find(id);
    if (it != orders_.end()) {
        return it->second;
    }
    return nullptr;
}

// int main() {
//     OrderManager<double, int> oms;

//     oms.create_order(1, 100.5, 10, true);
//     oms.create_order(2, 101.0, 5, false);

//     oms.fill_order(1, 5);  
//     oms.cancel_order(2);   

//     oms.print_all_orders();

//     return 0;
// }

template class OrderManager<double, int>;
