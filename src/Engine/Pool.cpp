#include "PE/Engine/Pool.h"

namespace PE::Engine {

    MemoryPool::~MemoryPool() {
        for(std::byte *ptr : m_memory) {
            delete[] ptr;
        }
    }

    template <typename T>
    ObjectPool<T>::~ObjectPool() {
        for(std::size_t i = 0; i < m_size; ++i) {
            T *object_ptr = static_cast<T>(get(i));
            object_ptr->~T();
        }
    }

}