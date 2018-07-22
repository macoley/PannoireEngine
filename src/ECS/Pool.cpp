#include "PE/ECS/Pool.h"

namespace PE::ECS {

    MemoryPool::~MemoryPool() {
        for(std::byte *ptr : m_memory) {
            delete[] ptr;
        }
    }

}