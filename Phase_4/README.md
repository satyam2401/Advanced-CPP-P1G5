# HFT_Project

## 📝 Design Overview

This High‑Frequency Trading (HFT) prototype is written in modern C++ (C++17) and focuses on low‑latency order ingestion, matching, and reporting. Key design goals:

- **Modularity**: Each component (market data feed, order management, order book, matching engine, trade logger) lives in its own module with clear public interfaces.
- **Performance**: Uses cache alignment (`alignas(64)`), custom memory pooling, and smart pointers to minimize allocation overhead.
- **Type Safety**: Leverages templates, `static_assert`, and compile‑time checks to enforce correct `PriceType` and `OrderIdType` usage.
- **Benchmarking**: Built‑in latency measurement (tick→trade) with nanosecond resolution using `std::chrono::high_resolution_clock`.

## 📐 System Architecture

```text
+------------------------+       +----------------------+       +----------------------+  
| MarketDataFeedSimulator|  -->  | OrderManager (OMS)   |       | TradeLogger          |
+------------------------+       +----------------------+       +----------------------+  
         |                              |                                ^           
         |                              v                                |           
         |                       +----------------+                     |           
         +-------------------->  | OrderBook      | <-------------------+           
                                 +----------------+                                 
                                         |                                         
                                         v                                         
                                 +----------------+                               
                                 | MatchingEngine |                               
                                 +----------------+                               
```  

**Module Responsibilities**

- **MarketDataFeedSimulator**: Generates aligned `MarketData` ticks (symbol, bid/ask, timestamp).
- **OrderManager**: Creates, updates, and cancels `Order` objects via `std::shared_ptr`.
- **OrderBook**: Maintains buy/sell sides using `std::multimap` and a `MemoryPool<Order>` for in‑place allocation.
- **MatchingEngine**: Continuously matches best bid and ask, updates fill status, and emits `Trade` records.
- **TradeLogger**: Buffers completed trades in `std::vector` (with `reserve`) and flushes to CSV in batches.
- **Timer**: Measures tick‑to‑trade latency in nanoseconds.

## 🛠️ Build Instructions

### Prerequisites

- C++17–compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or newer
- (Optional) `make` or other build tool

### Clone & Build

```bash
# Clone repository
git clone https://github.com/satyam2401/Advanced-CPP-P1G5
cd Advanced-CPP-P1G5/Phase_4

# Create build directory
mkdir build && cd build

# Configure and generate
cmake ..

# Compile
make
```

This produces two executables in `build/`:

- `hft_app` — a basic demo of order ingestion and matching.
- `latency_test` — measures tick‑to‑trade latency over N simulated ticks.

## ▶️ Run Examples

### hft_app

```bash
./hft_app
```

Prints a small sequence of orders and matches, demonstrating core functionality.

### latency_test

```bash
# Default: 100,000 ticks
./latency_test
```

Outputs:
```
Tick-to-Trade Latency (nanoseconds):
  Min   : <value>
  Max   : <value>
  Mean  : <value>
  StdDev: <value>
  P99   : <value>
```
And writes `trade_log.csv` containing all matched trades.

## 🔍 Performance Experiments

Modify parameters in `test_latency.cpp`:

- **Load scaling**: Change `numTicks` to 1K, 10K, 100K to test under different loads.
- **Memory allocation**: Toggle custom `MemoryPool` vs. `new/delete` in `OrderBook`.
- **Cache alignment**: Add/remove `alignas(64)` on `MarketData` or `Order`.
- **Container layout**: Swap `std::multimap` for flat vector + manual sort.

Record and compare latency statistics for each variant.
