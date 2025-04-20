#ifndef OrderManager_H
#define OrderManager_H
#include <string>
#include <memory>
#include <map>

enum class OrderStatus{New, Filled, ParticallyFilled, Cancelled};
enum class Side{Buy, Sell};

struct MyOrder
{    int id;
    Side side;
    double price;
    int quantity;
    int filled = 0;
    OrderStatus status = OrderStatus::New;

    MyOrder(int id_, Side s, double p, int q):
        id(id_), side(s), price(p), quantity(q){}
};

class OrderManager{
private:
    std::map<int, std::unique_ptr<MyOrder>> orders;
    int next_order_id = 1;
    static std::string to_string(Side s){
        return s == Side::Buy ? "Buy" : "Sell";
    };

public: 
    int place_order(Side side, double price, int qty);

    void cancel(int id);

    void handle_fill(int id, int filled_qty);

    void print_active_orders() const;
};


#endif 
