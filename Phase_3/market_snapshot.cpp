//
// Created by Satyam Saurabh on 19/04/25.
//

#include "market_snapshot.h"
#include "iostream"

void MarketSnapshot::update_bid(double price, int qty) {
    if (qty == 0) {
        bids.erase(price);
    } else {
        bids[price] = std::make_unique<PriceLevel>(price, qty);
    }
}

void MarketSnapshot::update_ask(double price, int qty) {
    if (qty == 0) {
        asks.erase(price);
    } else {
        asks[price] = std::make_unique<PriceLevel>(price, qty);
    }
}

const PriceLevel* MarketSnapshot::get_best_bid() const {
    if (bids.empty()) return nullptr;
    return bids.begin()->second.get();
}

const PriceLevel* MarketSnapshot::get_best_ask() const {
    if (asks.empty()) return nullptr;
    return asks.begin()->second.get();
}

void MarketSnapshot::print_book() const {
    if (const PriceLevel* bid = get_best_bid())
        std::cout << "[Book] Bid: " << bid->price << " x " << bid->quantity << "\n";
    if (const PriceLevel* ask = get_best_ask())
        std::cout << "[Book] Ask: " << ask->price << " x " << ask->quantity << "\n";
}