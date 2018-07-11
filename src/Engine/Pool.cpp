#include "PE/Engine/Pool.h"

namespace PE::Engine {

    MemoryPool::~MemoryPool() {
        for(std::byte *ptr : m_memory) {
            delete[] ptr;
        }
    }

}