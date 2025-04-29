#pragma
#include <vector>
#include <memory>

template <typename T>
class MemoryPool {
    std::vector<T> pool;
    std::vector<bool> in_use;

public:
    explicit MemoryPool(size_t size) : pool(size), in_use(size, false) {}

        template <typename... Args>
        T* allocate(Args&&... args) {
            for (size_t i = 0; i < pool.size(); ++i) {
                if (!in_use[i]) {
                    in_use[i] = true;
                    return &pool[i];
                }
            }
            return nullptr; // No available memory
        }

        void deallocate(T* ptr) {
            for (size_t i = 0; i < pool.size(); ++i){
                if (&pool[i] == ptr){
                    in_use[i] = false;
                    return;
                }
            }
        }
};
