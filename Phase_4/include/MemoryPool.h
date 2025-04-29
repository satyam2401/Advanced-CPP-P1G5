#pragma once

#include <vector>
#include <cstddef>
#include <type_traits>
#include <utility>

template <typename T>
class MemoryPool {
    struct Slot {
        alignas(alignof(T)) unsigned char data[sizeof(T)];
    };

    std::vector<Slot>   pool_;
    std::vector<bool>   in_use_;

public:
    explicit MemoryPool(std::size_t size)
      : pool_(size), in_use_(size,false) {}

    // construct a T with the given args in the first free slot
    template <typename... Args>
    T* allocate(Args&&... args) {
        for (std::size_t i = 0; i < pool_.size(); ++i) {
            if (!in_use_[i]) {
                in_use_[i] = true;
                void* addr = &pool_[i];
                // placement-new into the raw buffer
                return new (addr) T(std::forward<Args>(args)...);
            }
        }
        return nullptr;  // out of memory
    }

    // destroy the object and mark slot free
    void deallocate(T* ptr) {
        if (!ptr) return;
        // explicitly call destructor
        ptr->~T();

        // compute index by pointer arithmetic
        auto bytePtr = reinterpret_cast<unsigned char*>(ptr);
        auto base    = reinterpret_cast<unsigned char*>(pool_.data());
        std::size_t idx = (bytePtr - base) / sizeof(Slot);
        if (idx < in_use_.size()) {
            in_use_[idx] = false;
        }
    }
};
