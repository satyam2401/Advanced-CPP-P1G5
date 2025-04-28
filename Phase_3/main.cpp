#include "market_snapshot.h"
#include "order_manager.h"
#include "feed_parser.h"

#include <vector>
#include <fstream>
#include <sstream>

bool should_sell(const MarketSnapshot& snapshot) {
    const PriceLevel* best_bid = snapshot.get_best_bid();
    return best_bid && best_bid->price > 100.00;
}

int main() {
    // Use "../sample_feed.txt" and "../output.log" when running from IDE (e.g. CLion),
    // but use "sample_feed.txt" and "output.log" when running from terminal in project root.

    std::freopen("../output.log", "w", stdout);  // Redirect all std::cout to output.log

    MarketSnapshot snapshot;
    OrderManager om;
    auto feed = load_feed("../sample_feed.txt");
    double last_sell_price = -1.0;

    for (const auto& event : feed) {
        if (event.type == FeedType::BID) {
            snapshot.update_bid(event.price, event.quantity);
        } else if (event.type == FeedType::ASK) {
            snapshot.update_ask(event.price, event.quantity);
        } else if (event.type == FeedType::EXECUTION) {
            om.handle_fill(event.order_id, event.quantity);
        }

        if (should_sell(snapshot)) {
            const PriceLevel* best_bid = snapshot.get_best_bid();
            if (best_bid && best_bid->price != last_sell_price) {
                om.place_order(Side::Sell, best_bid->price, 50);
                last_sell_price = best_bid->price;
            }
        }

        snapshot.print_book();
    }

    om.print_active_orders();
    return 0;
}
