#pragma once
#include <array>
#include <atomic>
#include <cstddef>

// Alternative implementation with different memory ordering patterns
// for performance comparison
template <typename T, std::size_t Capacity>
class ExperimentalQueue {
public:
    ExperimentalQueue() : head{0}, tail{0} {}

    // Relaxed version - using mostly memory_order_relaxed
    bool push_relaxed(const T& item) {
        size_t currentTail = tail.load(std::memory_order_relaxed);
        size_t nextTail = (currentTail + 1) % Capacity;

        // Check if full
        if (nextTail == head.load(std::memory_order_relaxed))
            return false;  // Queue full

        buffer[currentTail] = item;
        tail.store(nextTail, std::memory_order_relaxed);
        return true;
    }

    bool pop_relaxed(T& item) {
        size_t currentHead = head.load(std::memory_order_relaxed);

        // Check if empty
        if (currentHead == tail.load(std::memory_order_relaxed))
            return false;  // Queue empty

        item = buffer[currentHead];
        head.store((currentHead + 1) % Capacity, std::memory_order_relaxed);
        return true;
    }

    // Sequential consistency version - using memory_order_seq_cst
    bool push_seq_cst(const T& item) {
        size_t currentTail = tail.load(std::memory_order_seq_cst);
        size_t nextTail = (currentTail + 1) % Capacity;

        // Check if full
        if (nextTail == head.load(std::memory_order_seq_cst))
            return false;  // Queue full

        buffer[currentTail] = item;
        tail.store(nextTail, std::memory_order_seq_cst);
        return true;
    }

    bool pop_seq_cst(T& item) {
        size_t currentHead = head.load(std::memory_order_seq_cst);

        // Check if empty
        if (currentHead == tail.load(std::memory_order_seq_cst))
            return false;  // Queue empty

        item = buffer[currentHead];
        head.store((currentHead + 1) % Capacity, std::memory_order_seq_cst);
        return true;
    }

    // Fence-based version - using explicit fences
    bool push_fenced(const T& item) {
        size_t currentTail = tail.load(std::memory_order_relaxed);
        size_t nextTail = (currentTail + 1) % Capacity;

        // Acquire fence before reading head
        std::atomic_thread_fence(std::memory_order_acquire);

        // Check if full
        if (nextTail == head.load(std::memory_order_relaxed))
            return false;  // Queue full

        buffer[currentTail] = item;

        // Release fence before updating tail
        std::atomic_thread_fence(std::memory_order_release);

        tail.store(nextTail, std::memory_order_relaxed);
        return true;
    }

    bool pop_fenced(T& item) {
        size_t currentHead = head.load(std::memory_order_relaxed);

        // Acquire fence before reading tail
        std::atomic_thread_fence(std::memory_order_acquire);

        // Check if empty
        if (currentHead == tail.load(std::memory_order_relaxed))
            return false;  // Queue empty

        item = buffer[currentHead];

        // Release fence before updating head
        std::atomic_thread_fence(std::memory_order_release);

        head.store((currentHead + 1) % Capacity, std::memory_order_relaxed);
        return true;
    }

private:
    std::array<T, Capacity> buffer;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
};