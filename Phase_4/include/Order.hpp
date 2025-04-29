#ifndef ORDER_HPP
#define ORDER_HPP
#include <type_traits>
#include <string>

enum class OrderStatus{
    New,
    PartiallyFilled,
    Filled,
    Cancelled
};

template <typename PriceType, typename OrderIdType>
class Order{
private:
    OrderIdType order_id_;
    std::string symbol_;
    PriceType price_;
    int quantity_;
    int filled_quantity_;
    bool is_buy_;
    OrderStatus status_;


public:
    static_assert(std::is_integral<OrderIdType>::value, "OrderId must be Integer");
    static_assert(std::is_floating_point<PriceType>::value, "PriceType must be Floating Point");

    Order() = default;
    Order(OrderIdType id, const std::string& symbol, PriceType price, int quantity, bool is_buy)
        : order_id_(id),symbol_(symbol), price_(price), quantity_(quantity), filled_quantity_(0), is_buy_(is_buy), status_(OrderStatus::New) {}

    void update_filled(int fill_quantity) {
        filled_quantity_ += fill_quantity;
        if (filled_quantity_ == 0) {
            status_ = OrderStatus::New;}
        else if (filled_quantity_ >= quantity_) {
            status_ = OrderStatus::Filled;
        } else {
            status_ = OrderStatus::PartiallyFilled;
        }
    }

    void cancel() {
        status_ = OrderStatus::Cancelled;
    }

    void set(OrderIdType id, const std::string& symbol, PriceType price, int quantity, bool is_buy) {
        order_id_ = id;
        symbol_ = symbol;
        price_ = price;
        quantity_ = quantity;
        is_buy_ = is_buy;
        filled_quantity_ = 0;
        status_ = OrderStatus::New;
    }

    OrderIdType get_id() const { return order_id_; }
    const std::string& get_symbol() const { return symbol_; }
    PriceType get_price() const { return price_; }
    int get_quantity() const { return quantity_; }
    int get_filled_quantity() const { return filled_quantity_; }
    bool is_buy_order() const { return is_buy_; }
    OrderStatus get_status() const { return status_; }
};

#endif