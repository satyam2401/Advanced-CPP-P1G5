// MemoryOrderingBenchmark.cpp
#include "ConcurrentQueue.hpp"
#include "ExperimentalQueue.hpp"
#include "OrderTypes.hpp"
#include "Benchmarks.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

// Number of operations to perform
constexpr int NUM_OPERATIONS = 100000;

// Function to benchmark different queue implementations
void benchmark_queues() {
    BenchmarkResults results;
    const int numRuns = 5;
    const int numProducers = 4;
    const int numConsumers = 4;

    std::cout << "=== Memory Ordering Benchmark ===\n\n";
    std::cout << "Testing different memory ordering strategies with "
              << NUM_OPERATIONS << " operations\n";
    std::cout << "Each test will be run " << numRuns << " times\n\n";

    for (int run = 0; run < numRuns; ++run) {
        std::cout << "Run " << (run + 1) << "/" << numRuns << ":\n";

        // Benchmark 1: Original Queue (mixed memory orderings)
        {
            std::cout << "  Testing Standard Queue (mixed memory ordering)... ";
            auto start = std::chrono::high_resolution_clock::now();

            // Create queue
            ConcurrentQueue<int, NUM_OPERATIONS> queue;

            // Create atomic flags for coordination
            std::atomic<bool> startFlag{false};
            std::atomic<int> produced{0};
            std::atomic<int> consumed{0};

            // Create threads
            std::vector<std::thread> producers;
            std::vector<std::thread> consumers;

            // Producer function
            auto producer_fn = [&](int id) {
                // Wait for start signal
                while (!startFlag.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }

                // Push items
                const int itemsPerProducer = NUM_OPERATIONS / numProducers;
                for (int i = 0; i < itemsPerProducer; ++i) {
                    int item = id * itemsPerProducer + i;
                    while (!queue.push(item)) {
                        std::this_thread::yield();
                    }
                    produced.fetch_add(1, std::memory_order_relaxed);
                }
            };

            // Consumer function
            auto consumer_fn = [&]() {
                // Wait for start signal
                while (!startFlag.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }

                // Pop items
                while (consumed.load(std::memory_order_relaxed) < NUM_OPERATIONS) {
                    int item;
                    if (queue.pop(item)) {
                        consumed.fetch_add(1, std::memory_order_relaxed);
                    } else {
                        std::this_thread::yield();
                    }
                }
            };

            // Create threads
            for (int i = 0; i < numProducers; ++i) {
                producers.emplace_back(producer_fn, i);
            }

            for (int i = 0; i < numConsumers; ++i) {
                consumers.emplace_back(consumer_fn);
            }

            // Start benchmark
            startFlag.store(true, std::memory_order_release);

            // Wait for threads to finish
            for (auto& t : producers) {
                t.join();
            }

            for (auto& t : consumers) {
                t.join();
            }

            // Record time
            auto end = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                end - start).count();

            std::cout << "Done in " << duration << " ms\n";
            results.addResult("Standard Queue", duration);
        }

        // Benchmark 2: Experimental Queue - Relaxed Version
        {
            std::cout << "  Testing Relaxed Queue (memory_order_relaxed)... ";
            auto start = std::chrono::high_resolution_clock::now();

            // Create queue
            ExperimentalQueue<int, NUM_OPERATIONS> queue;

            // Create atomic flags for coordination
            std::atomic<bool> startFlag{false};
            std::atomic<int> produced{0};
            std::atomic<int> consumed{0};

            // Create threads
            std::vector<std::thread> producers;
            std::vector<std::thread> consumers;

            // Producer function
            auto producer_fn = [&](int id) {
                // Wait for start signal
                while (!startFlag.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }

                // Push items
                const int itemsPerProducer = NUM_OPERATIONS / numProducers;
                for (int i = 0; i < itemsPerProducer; ++i) {
                    int item = id * itemsPerProducer + i;
                    while (!queue.push_relaxed(item)) {
                        std::this_thread::yield();
                    }
                    produced.fetch_add(1, std::memory_order_relaxed);
                }
            };

            // Consumer function
            auto consumer_fn = [&]() {
                // Wait for start signal
                while (!startFlag.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }

                // Pop items
                while (consumed.load(std::memory_order_relaxed) < NUM_OPERATIONS) {
                    int item;
                    if (queue.pop_relaxed(item)) {
                        consumed.fetch_add(1, std::memory_order_relaxed);
                    } else {
                        std::this_thread::yield();
                    }
                }
            };

            // Create threads
            for (int i = 0; i < numProducers; ++i) {
                producers.emplace_back(producer_fn, i);
            }

            for (int i = 0; i < numConsumers; ++i) {
                consumers.emplace_back(consumer_fn);
            }

            // Start benchmark
            startFlag.store(true, std::memory_order_release);

            // Wait for threads to finish
            for (auto& t : producers) {
                t.join();
            }

            for (auto& t : consumers) {
                t.join();
            }

            // Record time
            auto end = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                end - start).count();

            std::cout << "Done in " << duration << " ms\n";
            results.addResult("Relaxed Queue", duration);
        }

        // Benchmark 3: Experimental Queue - Sequential Consistency Version
        {
            std::cout << "  Testing Sequential Consistency Queue (memory_order_seq_cst)... ";
            auto start = std::chrono::high_resolution_clock::now();

            // Create queue
            ExperimentalQueue<int, NUM_OPERATIONS> queue;

            // Create atomic flags for coordination
            std::atomic<bool> startFlag{false};
            std::atomic<int> produced{0};
            std::atomic<int> consumed{0};

            // Create threads
            std::vector<std::thread> producers;
            std::vector<std::thread> consumers;

            // Producer function
            auto producer_fn = [&](int id) {
                // Wait for start signal
                while (!startFlag.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }

                // Push items
                const int itemsPerProducer = NUM_OPERATIONS / numProducers;
                for (int i = 0; i < itemsPerProducer; ++i) {
                    int item = id * itemsPerProducer + i;
                    while (!queue.push_seq_cst(item)) {
                        std::this_thread::yield();
                    }
                    produced.fetch_add(1, std::memory_order_relaxed);
                }
            };

            // Consumer function
            auto consumer_fn = [&]() {
                // Wait for start signal
                while (!startFlag.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }

                // Pop items
                while (consumed.load(std::memory_order_relaxed) < NUM_OPERATIONS) {
                    int item;
                    if (queue.pop_seq_cst(item)) {
                        consumed.fetch_add(1, std::memory_order_relaxed);
                    } else {
                        std::this_thread::yield();
                    }
                }
            };

            // Create threads
            for (int i = 0; i < numProducers; ++i) {
                producers.emplace_back(producer_fn, i);
            }

            for (int i = 0; i < numConsumers; ++i) {
                consumers.emplace_back(consumer_fn);
            }

            // Start benchmark
            startFlag.store(true, std::memory_order_release);

            // Wait for threads to finish
            for (auto& t : producers) {
                t.join();
            }

            for (auto& t : consumers) {
                t.join();
            }

            // Record time
            auto end = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                end - start).count();

            std::cout << "Done in " << duration << " ms\n";
            results.addResult("Sequential Consistency Queue", duration);
        }

        // Benchmark 4: Experimental Queue - Fence-Based Version
        {
            std::cout << "  Testing Fence-Based Queue (explicit fences)... ";
            auto start = std::chrono::high_resolution_clock::now();

            // Create queue
            ExperimentalQueue<int, NUM_OPERATIONS> queue;

            // Create atomic flags for coordination
            std::atomic<bool> startFlag{false};
            std::atomic<int> produced{0};
            std::atomic<int> consumed{0};

            // Create threads
            std::vector<std::thread> producers;
            std::vector<std::thread> consumers;

            // Producer function
            auto producer_fn = [&](int id) {
                // Wait for start signal
                while (!startFlag.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }

                // Push items
                const int itemsPerProducer = NUM_OPERATIONS / numProducers;
                for (int i = 0; i < itemsPerProducer; ++i) {
                    int item = id * itemsPerProducer + i;
                    while (!queue.push_fenced(item)) {
                        std::this_thread::yield();
                    }
                    produced.fetch_add(1, std::memory_order_relaxed);
                }
            };

            // Consumer function
            auto consumer_fn = [&]() {
                // Wait for start signal
                while (!startFlag.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }

                // Pop items
                while (consumed.load(std::memory_order_relaxed) < NUM_OPERATIONS) {
                    int item;
                    if (queue.pop_fenced(item)) {
                        consumed.fetch_add(1, std::memory_order_relaxed);
                    } else {
                        std::this_thread::yield();
                    }
                }
            };

            // Create threads
            for (int i = 0; i < numProducers; ++i) {
                producers.emplace_back(producer_fn, i);
            }

            for (int i = 0; i < numConsumers; ++i) {
                consumers.emplace_back(consumer_fn);
            }

            // Start benchmark
            startFlag.store(true, std::memory_order_release);

            // Wait for threads to finish
            for (auto& t : producers) {
                t.join();
            }

            for (auto& t : consumers) {
                t.join();
            }

            // Record time
            auto end = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                end - start).count();

            std::cout << "Done in " << duration << " ms\n";
            results.addResult("Fence-Based Queue", duration);
        }

        std::cout << std::endl;
    }

    // Print final results
    results.printResults();

    std::cout << "\nMemory Ordering Analysis:\n";
    std::cout << "- memory_order_relaxed: Fastest but provides minimal guarantees\n";
    std::cout << "- memory_order_seq_cst: Slowest but strongest guarantees\n";
    std::cout << "- Mixed orderings: Balance between performance and correctness\n";
    std::cout << "- Explicit fences: Can be more efficient than per-operation ordering\n";
}

int main() {
    std::cout << "Starting Memory Ordering Benchmark\n\n";
    benchmark_queues();

    return 0;
}