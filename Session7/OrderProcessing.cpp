#include "ConcurrentQueue.hpp"
#include "OrderTypes.hpp"
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <semaphore>
#include <barrier>
#include <latch>
#include <atomic>

// Global concurrent queue for orders
constexpr size_t QUEUE_CAPACITY = 1024;
ConcurrentQueue<Order, QUEUE_CAPACITY> orderQueue;

// Global order book, mapping symbol to (buy volume, sell volume)
std::unordered_map<std::string, std::pair<int, int>> orderBook;
std::shared_mutex orderBookMutex; // Reader-writer lock

// Atomic counter to measure total processed orders
std::atomic<int> ordersProcessed{0};

// Vector to store completed orders for analytics
std::vector<Order> completedOrders;
std::mutex completedOrdersMutex;

// For demonstration, limit concurrent order processing (max 3 orders in parallel)
std::counting_semaphore<3> processingSemaphore(3);

// Barrier to synchronize consumer threads after a batch round (e.g., 4 threads)
std::barrier syncBarrier(4);

// Latch to ensure all threads start processing at the same time
std::latch startLatch(1);

// Producer thread function: simulates incoming orders
void orderProducer(int producerId, int numOrders) {
    // Wait until all threads are ready to start
    startLatch.wait();

    for (int i = 0; i < numOrders; ++i) {
        Order order;
        order.orderId = producerId * 1000 + i;
        order.symbol = (i % 2 == 0) ? "APPL" : "GOOGL";
        order.quantity = 100 + i;
        order.price = (order.symbol == "APPL") ? 146.23 + i * 0.01 : 2800.5 + i * 0.05;
        order.isBuy = (i % 2 == 0);
        order.enqueueTime = std::chrono::high_resolution_clock::now();

        // Try to push; if full, keep trying (busy-wait for simplicity)
        while (!orderQueue.push(order)) {
            std::this_thread::yield();
        }

        // Simulate some time between orders
        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
}

// Consumer thread function: processes orders from the queue
void orderConsumer() {
    // Wait until producers start
    startLatch.wait();

    while (true) {
        Order order;

        // If queue is empty and enough orders processed, break out
        if (!orderQueue.pop(order)) {
            if (ordersProcessed.load(std::memory_order_acquire) >= 50) {
                break;  // Assume total of 50 orders
            }
            std::this_thread::yield();
            continue;
        }

        order.dequeueTime = std::chrono::high_resolution_clock::now();
        processingSemaphore.acquire();  // Restrict parallel processing count

        // Simulate order processing latency (for example, risk validation, etc.)
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        // Update global order book (exclusive write via unique_lock)
        {
            std::unique_lock<std::shared_mutex> lock(orderBookMutex);
            auto& entry = orderBook[order.symbol];
            if (order.isBuy)
                entry.first += order.quantity;   // Update buy volume
            else
                entry.second += order.quantity;  // Update sell volume
        }

        // Store completed order for analytics
        {
            std::lock_guard<std::mutex> lock(completedOrdersMutex);
            completedOrders.push_back(order);
        }

        ordersProcessed.fetch_add(1, std::memory_order_release);
        processingSemaphore.release();  // Release semaphore

        // Wait at barrier after each order
        syncBarrier.arrive_and_wait();
    }
}

// Function to record a processed order
void recordOrder(const Order& order) {
    std::lock_guard<std::mutex> lock(completedOrdersMutex);
    completedOrders.push_back(order);
}

// Get a copy of completed orders (for analytics)
std::vector<Order> getCompletedOrders() {
    std::lock_guard<std::mutex> lock(completedOrdersMutex);
    return completedOrders;
}