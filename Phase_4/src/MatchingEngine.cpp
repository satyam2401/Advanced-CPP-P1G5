#include "OrderManager.h"
#include "OrderBook.h"
#include "MatchingEngine.h"
#include <iostream>

template<typename PriceType, typename OrderIdType>
MatchingEngine<PriceType, OrderIdType>::MatchingEngine(OrderBookType& order_book)
    : order_book_(order_book) {}

template<typename PriceType, typename OrderIdType>
std::vector<typename MatchingEngine<PriceType, OrderIdType>::TradeType>
MatchingEngine<PriceType, OrderIdType>::match_orders(){
    std::vector<TradeType> trades;

    while (true) {
        auto best_bid = order_book_.get_best_bid();
        auto best_ask = order_book_.get_best_ask();

        if (!best_bid || !best_ask) break;
        if (best_bid->get_price() < best_ask->get_price()) break;

        int trade_qty = std::min(best_bid->get_quantity(), best_ask->get_quantity());
        PriceType trade_price = best_ask->get_price();

        trades.push_back({best_bid->get_id(), best_ask->get_id(), trade_price, trade_qty});

        best_bid->update_filled(trade_qty);
        best_ask->update_filled(trade_qty);

        if (best_bid->get_status() == OrderStatus::Filled) {
            order_book_.remove_order(best_bid->get_id());
        }

        if (best_ask->get_status() == OrderStatus::Filled) {
            order_book_.remove_order(best_ask->get_id());
        }
    }

    return trades;
}

template class MatchingEngine<double, int>;

// int main() {
//     using PriceType = double;
//     using OrderIdType = int;

//     OrderManager<PriceType, OrderIdType> oms;
//     OrderBook<PriceType, OrderIdType> order_book;
//     MatchingEngine<PriceType, OrderIdType> engine(order_book);

//     oms.create_order(1, 100.5, 10, true);    // Buy order
//     oms.create_order(2, 101.0, 5, true);      // Buy order
//     oms.create_order(3, 100.0, 20, false);    // Sell order
//     oms.create_order(4, 103.5, 15, false);    // Sell order

//     auto order1 = oms.get_order(1);
//     auto order2 = oms.get_order(2);
//     auto order3 = oms.get_order(3);
//     auto order4 = oms.get_order(4);

//     order_book.add_order(order1->get_id(), order1->get_price(), order1->get_quantity(), order1->is_buy_order());
//     order_book.add_order(order2->get_id(), order2->get_price(), order2->get_quantity(), order2->is_buy_order());
//     order_book.add_order(order3->get_id(), order3->get_price(), order3->get_quantity(), order3->is_buy_order());
//     order_book.add_order(order4->get_id(), order4->get_price(), order4->get_quantity(), order4->is_buy_order());

//     auto trades = engine.match_orders();

//     std::cout << "Matched Trades:\n";
//     for (const auto& trade : trades) {
//         std::cout << "BuyOrderID: " << trade.buy_order_id
//                   << ", SellOrderID: " << trade.sell_order_id
//                   << ", Price: " << trade.price
//                   << ", Quantity: " << trade.quantity
//                   << std::endl;
//     }

//     return 0;
// }

