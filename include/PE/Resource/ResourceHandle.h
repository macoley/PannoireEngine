#ifndef PANNOIREENGINE_RESOURCEHANDLE_H
#define PANNOIREENGINE_RESOURCEHANDLE_H

#include <cstdint>

#include "ResourcePool.h"

namespace PE::Resource {

    template<typename Resource>
    class ResourcePool;

    /**
     * Resource Handle
     * @tparam Resource
     */
    template<typename Resource>
    class ResourceHandle {
        using Handle = ResourceHandle<Resource>;
        using PoolPtr = std::shared_ptr<ResourcePool<Resource>>;

    public:
        ResourceHandle(ResourceIndex index, PoolPtr pool)
                : m_index(index),
                  m_pool(pool)
        {
            pool->incrementCounter(index);
        }

        virtual ~ResourceHandle() {
            m_pool->decrementCounter(m_index);
        }

        // Copy Constructor
        ResourceHandle(const ResourceHandle<Resource> &other)
                : m_index(other.m_index),
                  m_pool(other.m_pool)
        {
            m_pool->incrementCounter(m_index);
        }

        ResourceHandle<Resource>& operator=(const ResourceHandle<Resource>& other)
        {
            if (this != &other)
            {
                // decrement old one
                m_pool->decrementCounter(m_index);

                m_index = other.m_index;
                m_pool = other.m_pool; // pool independent

                // increment new one
                m_pool->incrementCounter(m_index);
            }

            return *this;
        }

        Resource& operator*() {
            return *(m_pool->get(m_index));
        }

        Resource* operator->() {
            return m_pool->get(m_index);
        }

        std::size_t getRefCount() const {
            return m_pool->getCount(m_index);
        }

    private:
        ResourceIndex m_index;
        PoolPtr m_pool;
    };

}

#endif //PANNOIREENGINE_RESOURCEHANDLE_H
