#include <iostream>
#include <fstream>
#include "feed_parser.h"
#include "market_snapshot.h"
#include "order_manager.h"

std:: ofstream log_file;
void log(const std::string& msg) {
    std::cout << msg << '\n';
    if (log_file.is_open()) {
        log_file << msg << std::endl;
    }
}

void trading_strategy(const MarketSnapshot& snapshot, OrderManager& order_manager){
    const PriceLevel* bid = snapshot.get_best_bid();
    const PriceLevel* ask = snapshot.get_best_ask();

    if (!bid || !ask) return;

    if (bid -> price > 100.00){
        log("Condition met: best bid = " + std::to_string(bid->price) + 
        " > 100 → placing SELL order for 10 shares");

        order_manager.place_order(Side::Sell, bid -> price, 10);
    }
}
int main(){
    MarketSnapshot snapshot;
    OrderManager order_manager;
    log_file.open("output.log");
    auto feed = load_feed("sample_feed.text");

    for (const auto& event:feed){
        event.print();
        if (event.type == FeedType::BID){
            snapshot.update_bid(event.price, event.quantity);
            log("[FEED] BID " + std::to_string(event.price) + 
            " × " + std::to_string(event.quantity));
        } else if (event.type == FeedType::ASK){
            snapshot.update_ask(event.price, event.quantity);
            log("[FEED] ASK " + std::to_string(event.price) + 
            " × " + std::to_string(event.quantity));
        } else if (event.type == FeedType::EXECUTION){
            order_manager.handle_fill(event.order_id, event.quantity);
            log("[FEED] EXECUTION ID " + std::to_string(event.order_id) +
                " × " + std::to_string(event.quantity));
        }
        trading_strategy(snapshot, order_manager);
    }

    return 0;
}