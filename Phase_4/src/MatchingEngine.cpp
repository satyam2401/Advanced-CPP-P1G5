#include "OrderManager.hpp"
#include "OrderBook.hpp"
#include "MatchingEngine.hpp"
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

/// int main() {
//     using PriceType = double;
//     using OrderIdType = int;
//
//     OrderManager<PriceType,OrderIdType> oms;
//     OrderBook<PriceType,OrderIdType>   order_book;
//     MatchingEngine<PriceType,OrderIdType> engine(order_book);
//
//     oms.create_order(1, "AAPL", 100.5, 10, true);
//     oms.create_order(2, "AAPL", 101.0, 5, true);
//     oms.create_order(3, "AAPL", 100.0, 20, false);
//     oms.create_order(4, "AAPL", 103.5, 15, false);
//
//     auto o1 = oms.get_order(1);
//     auto o2 = oms.get_order(2);
//     auto o3 = oms.get_order(3);
//     auto o4 = oms.get_order(4);
//
//     order_book.add_order(o1->get_id(),
//                          o1->get_symbol(),
//                          o1->get_price(),
//                          o1->get_quantity(),
//                          o1->is_buy_order());
//
//     order_book.add_order(o2->get_id(),
//                          o2->get_symbol(),
//                          o2->get_price(),
//                          o2->get_quantity(),
//                          o2->is_buy_order());
//
//     order_book.add_order(o3->get_id(),
//                          o3->get_symbol(),
//                          o3->get_price(),
//                          o3->get_quantity(),
//                          o3->is_buy_order());
//
//     order_book.add_order(o4->get_id(),
//                          o4->get_symbol(),
//                          o4->get_price(),
//                          o4->get_quantity(),
//                          o4->is_buy_order());
//
//     auto trades = engine.match_orders();
//
//     std::cout << "Matched Trades:\n";
//     for (const auto& tr : trades) {
//         std::cout << "BuyID:" << tr.buy_order_id
//                   << " SellID:" << tr.sell_order_id
//                   << " Price:"  << tr.price
//                   << " Qty:"    << tr.quantity
//                   << "\n";
//     }
//
//     return 0;
// }

