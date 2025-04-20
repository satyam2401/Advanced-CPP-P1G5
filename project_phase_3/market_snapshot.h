#ifndef MarketSnapshot_H
#define MarketSnapshot_H

#include <memory>
#include <map>


struct PriceLevel{
    double price;
    int quantity;

    PriceLevel(double p, int q): price(p), quantity(q) {}
};

class MarketSnapshot {
private:
    std::map<double, std::unique_ptr<PriceLevel>, std::greater<>> bids;
    std::map<double, std::unique_ptr<PriceLevel>> asks;

public:
    void update_bid(double price, int qty);

    void update_ask(double price, int qty);


    const PriceLevel* get_best_bid() const;

    const PriceLevel* get_best_ask() const;

    void print_top_of_book() const;
};


#endif