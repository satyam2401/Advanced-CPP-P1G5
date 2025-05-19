#pragma once
#include <array>
#include <atomic>
#include <cstddef>

template <typename T, std::size_t Capacity>
class ConcurrentQueue {
public:
    ConcurrentQueue() : head{0}, tail{0} {}

    // Push an item into the queue. Returns false if the queue is full.
    bool push(const T& item) {
        size_t currentTail = tail.load(std::memory_order_relaxed);
        size_t nextTail = (currentTail + 1) % Capacity;

        // Check if full: nextTail catches up to head
        if (nextTail == head.load(std::memory_order_acquire))
            return false;  // Queue full

        buffer[currentTail] = item;

        // Ensure the item write happens before tail is advanced
        std::atomic_thread_fence(std::memory_order_release);

        tail.store(nextTail, std::memory_order_release);
        return true;
    }

    // Pop an item from the queue. Returns false if the queue is empty.
    bool pop(T& item) {
        size_t currentHead = head.load(std::memory_order_relaxed);

        // Check if empty: head catches up to tail
        if (currentHead == tail.load(std::memory_order_acquire))
            return false;  // Queue empty

        item = buffer[currentHead];

        // Ensure item read happens before head is advanced
        std::atomic_thread_fence(std::memory_order_release);

        head.store((currentHead + 1) % Capacity, std::memory_order_release);
        return true;
    }

    // Check if queue is empty
    bool empty() const {
        return head.load(std::memory_order_acquire) ==
               tail.load(std::memory_order_acquire);
    }

    // Check if queue is full
    bool full() const {
        return (tail.load(std::memory_order_acquire) + 1) % Capacity ==
               head.load(std::memory_order_acquire);
    }

    // Get current size (approximate due to concurrent nature)
    size_t size() const {
        size_t h = head.load(std::memory_order_acquire);
        size_t t = tail.load(std::memory_order_acquire);

        if (t >= h)
            return t - h;
        else
            return Capacity - h + t;
    }

private:
    std::array<T, Capacity> buffer;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
};