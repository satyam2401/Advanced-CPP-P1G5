//
// Created by Satyam Saurabh on 19/04/25.
//

#ifndef PHASE_3_MARKET_SNAPSHOT_H
#define PHASE_3_MARKET_SNAPSHOT_H

#include <map>
#include <memory>

struct PriceLevel {
    double price;
    int quantity;

    PriceLevel(double p, int q) : price(p), quantity(q) {}
};

class MarketSnapshot {
    std::map<double, std::unique_ptr<PriceLevel>, std::greater<>> bids;
    std::map<double, std::unique_ptr<PriceLevel>> asks;

public:
    void update_bid(double price, int qty);
    void update_ask(double price, int qty);
    [[nodiscard]] const PriceLevel* get_best_bid() const;
    [[nodiscard]] const PriceLevel* get_best_ask() const;
    void print_book() const;
};


#endif //PHASE_3_MARKET_SNAPSHOT_H
