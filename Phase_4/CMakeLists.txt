cmake_minimum_required(VERSION 3.10)
project(HFT_Project)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(hft_app
    src/main.cpp
    src/MarketData.cpp
    src/OrderBook.cpp
    src/MatchingEngine.cpp
    src/OrderManager.cpp
    src/TradeLogger.cpp
    src/MarketDataFeedSimulator.cpp
)

add_executable(latency_test
        test/test_latency.cpp
        src/MarketData.cpp
        src/MarketDataFeedSimulator.cpp
        src/OrderBook.cpp
        src/OrderManager.cpp
        src/MatchingEngine.cpp
        src/TradeLogger.cpp
)
