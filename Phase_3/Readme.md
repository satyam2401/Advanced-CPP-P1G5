# Trading System: Market Snapshot + Order Manager + Strategy Execution

## Overview

This project simulates a basic real-time trading system that processes market data (bid/ask updates), manages orders, and executes trading logic. It is composed of three main components:

- **MarketSnapshot**: Maintains the current view of the order book (best bid/ask).
- **OrderManager**: Tracks all active orders, their states, fills, and cancellations.
- **Strategy Loop**: Reacts to market data and decides when to place orders.
- **FeedParser**: Reads structured input from `sample_feed.txt` and drives the system.

All system decisions and updates are logged to `output.log` for post-run inspection.

---

## Memory Safety Design

This system is written in modern C++ with **RAII** and smart pointers (`std::unique_ptr`) to ensure safe memory handling.

- All dynamically allocated `PriceLevel` and `MyOrder` objects are stored in `std::unique_ptr`.
- When price levels or orders are removed, their memory is released automatically.
- No use of `new` or `delete` — all heap memory is managed by smart pointers.
- STL containers (`std::map`) ensure proper ownership and automatic cleanup.
- No raw pointers are used to manage ownership. Returned pointers from accessors (like `get_best_bid()`) are non-owning and read-only.

---

## Compilation & Execution Instructions

### Build & Run

#### Using g++:

```bash
g++ -std=c++17 -fsanitize=address *.cpp -o trade_sim
./trade_sim
