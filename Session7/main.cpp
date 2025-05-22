#include "ConcurrentQueue.hpp"
#include "OrderTypes.hpp"
#include "Benchmarks.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <latch>
#include <shared_mutex>
#include <unordered_map>

// Forward declarations
void orderProducer(int producerId, int numOrders);
void orderConsumer();
void runAnalytics();

// Externals defined in OrderProcessing.cpp
extern std::atomic<int>                ordersProcessed;
extern std::latch                      startLatch;
extern std::atomic<bool>               producersDone;
extern std::shared_mutex               orderBookMutex;
extern std::unordered_map<std::string, std::pair<int,int>> orderBook;

int main() {
    BenchmarkResults benchmarks;

    std::cout << "=== Trade Processing System with Concurrent Queue ===\n\n";
    const int numProducers  = 5;
    const int ordersPerProd = 10;  // total = 50
    const int numConsumers  = 4;

    // launch producers + consumers
    std::vector<std::thread> producers, consumers;
    for (int i = 0; i < numProducers; ++i)
        producers.emplace_back(orderProducer, i+1, ordersPerProd);
    for (int i = 0; i < numConsumers; ++i)
        consumers.emplace_back(orderConsumer);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Starting order processing...\n";
    startLatch.count_down();

    // wait for all producers
    for (auto &p : producers) p.join();
    std::cout << "All producers completed\n";

    // signal consumers no more enqueues
    producersDone.store(true, std::memory_order_release);

    // wait for consumers to finish
    for (auto &c : consumers) c.join();
    std::cout << "All consumers completed\n";

    // ... timing, benchmarks, analytics, summary ...
    {
        BenchmarkTimer timer("Analytics Execution");
        runAnalytics();
    }

    std::shared_lock lock(orderBookMutex);
    std::cout << "\n--- Order Book Summary ---\n";
    for (auto const& [sym, vol] : orderBook) {
        std::cout << "Symbol: " << sym
                  << " | Buy: "  << vol.first
                  << " | Sell: " << vol.second << "\n";
    }

    return 0;
}
