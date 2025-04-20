#include "market_snapshot.h"
#include <memory>
#include<iostream>
#include<iomanip>

void MarketSnapshot::update_bid(double price, int qty){
    if (qty <= 0){
        bids.erase(price); //cancel order
    } else {
        bids[price] = std::make_unique<PriceLevel> (price, qty); 
    }
}
void MarketSnapshot::update_ask(double price, int qty){
    if (qty <= 0){
        asks.erase(price);
    } else {
        asks[price] = std::make_unique<PriceLevel> (price, qty);
    }
}

const PriceLevel* MarketSnapshot::get_best_bid() const{
    if (bids.empty()) return nullptr;
    return bids.begin() -> second.get();
}

const PriceLevel* MarketSnapshot::get_best_ask() const{
    if (asks.empty()) return nullptr;
    return asks.begin() -> second.get();
}

void MarketSnapshot::print_top_of_book() const{
    const PriceLevel* bid = get_best_bid();
    const PriceLevel* ask = get_best_ask();

    std::cout << "\n ---- Top of Book ---\n";
    if (bid){
        std::cout << "Best Bid: " << std::fixed << std::setprecision(2) << bid->price << "x" << bid->quantity << "\n";
    } else {
        std::cout << "No bids.\n";
    }

    if (ask){
        std::cout << "Best Ask: " << std::fixed << std::setprecision(2) << ask->price << "x" << ask->quantity << "\n";
    } else {
        std::cout << "No asks.\n";
    }
    std::cout << "-----------\n";
}

// int main() {
//     MarketSnapshot book;

//     // initial market
//     book.update_bid(100.10, 300);
//     book.update_ask(100.20, 250);
//     book.print_top_of_book();

//     // new better price 
//     book.update_bid(100.15, 200);
//     book.update_ask(100.18, 100);
//     book.print_top_of_book();

//     // partial change
//     book.update_bid(100.15, 20);     // 删除老 bid
//     book.update_ask(100.18, 60);    // 修改 ask 数量
//     book.print_top_of_book();

//     // full cancel
//     book.update_ask(100.18, 0);
//     book.print_top_of_book();

//     return 0;
// }