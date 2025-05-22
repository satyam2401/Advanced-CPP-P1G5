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
#include <latch>
#include <atomic>
#include <chrono>

// Global concurrent queue for orders
constexpr size_t QUEUE_CAPACITY = 1024;
ConcurrentQueue<Order, QUEUE_CAPACITY> orderQueue;

// Global order book, mapping symbol → (buy volume, sell volume)
std::unordered_map<std::string, std::pair<int,int>> orderBook;
std::shared_mutex orderBookMutex;

// Atomic counter to measure total processed orders
std::atomic<int> ordersProcessed{0};

// Completed orders for analytics
std::vector<Order> completedOrders;
std::mutex     completedOrdersMutex;

// Control max parallel processing (3 at a time)
std::counting_semaphore<3> processingSemaphore(3);

// Latch to sync startup of all threads
std::latch startLatch(1);

// **New** flag: signals no more pushes will occur
std::atomic<bool> producersDone{false};

// Producer thread: enqueues `numOrders` orders
void orderProducer(int producerId, int numOrders) {
    startLatch.wait();
    for (int i = 0; i < numOrders; ++i) {
        Order order;
        order.orderId     = producerId * 1000 + i;
        order.symbol      = (i % 2 == 0) ? "APPL" : "GOOGL";
        order.quantity    = 100 + i;
        order.price       = (order.symbol=="APPL")
                            ? 146.23 + i * 0.01
                            : 2800.5 + i * 0.05;
        order.isBuy       = (i % 2 == 0);
        order.enqueueTime = std::chrono::high_resolution_clock::now();

        while (!orderQueue.push(order))
            std::this_thread::yield();

        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
}

// Consumer thread: dequeues and processes until `producersDone` + queue empty
void orderConsumer() {
    startLatch.wait();
    while (true) {
        Order order;
        if (!orderQueue.pop(order)) {
            // exit when no more producers and queue drained
            if (producersDone.load(std::memory_order_acquire))
                break;
            std::this_thread::yield();
            continue;
        }

        order.dequeueTime = std::chrono::high_resolution_clock::now();
        processingSemaphore.acquire();
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        // update shared order book
        {
            std::unique_lock lock(orderBookMutex);
            auto &entry = orderBook[order.symbol];
            if (order.isBuy)  entry.first  += order.quantity;
            else              entry.second += order.quantity;
        }

        // record for analytics
        {
            std::lock_guard lock(completedOrdersMutex);
            completedOrders.push_back(order);
        }

        ordersProcessed.fetch_add(1, std::memory_order_relaxed);
        processingSemaphore.release();
    }
}

// Snapshot of completed orders for Analytics.cpp
std::vector<Order> getCompletedOrders() {
    std::lock_guard lock(completedOrdersMutex);
    return completedOrders;
}
