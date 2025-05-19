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
#include <execution>
#include <algorithm>


// Forward declarations
void orderProducer(int producerId, int numOrders);
void orderConsumer();
void runAnalytics();

// External declarations from OrderProcessing.cpp
extern std::atomic<int> ordersProcessed;
extern std::latch startLatch;

int main() {
    // Benchmark results collector
    BenchmarkResults benchmarks;
    
    std::cout << "=== Trade Processing System with Concurrent Queue ===\n\n";
    
    // Configuration
    const int numProducers = 5;
    const int ordersPerProducer = 10; // Total orders = 50
    const int numConsumers = 4;
    
    // Timing the entire process
    auto mainStart = std::chrono::high_resolution_clock::now();
    
    std::cout << "Creating " << numProducers << " producers and " 
              << numConsumers << " consumers...\n";
    
    // Create producer and consumer threads
    std::vector<std::thread> producers, consumers;
    
    // Launch producer threads
    for (int i = 0; i < numProducers; ++i) {
        producers.emplace_back(orderProducer, i + 1, ordersPerProducer);
    }
    
    // Launch consumer threads
    for (int i = 0; i < numConsumers; ++i) {
        consumers.emplace_back(orderConsumer);
    }
    
    // Give threads time to initialize
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Start the benchmark timer
    auto processingStart = std::chrono::high_resolution_clock::now();
    
    // Start all threads simultaneously with the latch
    std::cout << "Starting order processing...\n";
    startLatch.count_down();
    
    // Join producer threads
    for (auto& p : producers) {
        p.join();
    }
    std::cout << "All producers completed\n";
    
    // Wait for consumers to finish processing
    for (auto& c : consumers) {
        c.join();
    }
    std::cout << "All consumers completed\n";
    
    // Stop the benchmark timer
    auto processingEnd = std::chrono::high_resolution_clock::now();
    double processingTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        processingEnd - processingStart).count();
    
    std::cout << "Total orders processed: " << ordersProcessed.load() << "\n";
    std::cout << "Processing time: " << processingTime << " ms\n";
    
    // Record benchmark result
    benchmarks.addResult("Full Order Processing", processingTime);
    
    // Run analytics on the processed orders
    {
        BenchmarkTimer analyticsTimer("Analytics Execution Time");
        runAnalytics();
    }
    
    // Display order book summary
    {
        extern std::shared_mutex orderBookMutex;
        extern std::unordered_map<std::string, std::pair<int, int>> orderBook;
        
        std::shared_lock<std::shared_mutex> lock(orderBookMutex);
        std::cout << "\n--- Order Book Summary ---\n";
        for (const auto& [symbol, volume] : orderBook) {
            std::cout << "Symbol: " << symbol
                    << " | Buy Volume: " << volume.first
                    << " | Sell Volume: " << volume.second << "\n";
        }
    }
    
    // Calculate overall execution time
    auto mainEnd = std::chrono::high_resolution_clock::now();
    double totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        mainEnd - mainStart).count();
    
    std::cout << "\nTotal execution time: " << totalTime << " ms\n";
    
    // Print all benchmark results
    benchmarks.printResults();
    
    std::cout << "\nMemory Ordering Notes:\n"
              << "- memory_order_relaxed used for simple counter updates\n"
              << "- memory_order_acquire used when reading queue state\n"
              << "- memory_order_release used when writing new data\n"
              << "- std::atomic_thread_fence used for explicit barriers\n";
    
    std::cout << "\nSynchronization Primitives Used:\n"
              << "- std::shared_mutex for concurrent order book access\n"
              << "- std::barrier for consumer thread synchronization\n"
              << "- std::latch for coordinated thread startup\n"
              << "- std::counting_semaphore to limit concurrent processing\n";
    
    std::cout << "\nCompleted benchmark suite successfully!\n";
    
    return 0;
}