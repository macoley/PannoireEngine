#ifndef PE_RES_RESOURCEHANDLE_H
#define PE_RES_RESOURCEHANDLE_H

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
        ResourceHandle() = default;

        ResourceHandle(ResourceIndex index, PoolPtr pool)
                : m_index(index),
                  m_pool(pool),
                  m_weak(false) {
            increment();
        }

        virtual ~ResourceHandle() {
            decrement();
        }

        // Copy Constructor
        ResourceHandle(const ResourceHandle<Resource> &other)
                : m_index(other.m_index),
                  m_pool(other.m_pool),
                  m_weak(other.m_weak) {
            increment();
        }

        ResourceHandle<Resource> &operator=(const ResourceHandle<Resource> &other) {
            if (this != &other) {
                // decrement old one
                decrement();

                m_index = other.m_index;
                m_pool = other.m_pool; // pool independent
                m_weak = other.m_weak;

                // increment new one
                increment();
            }

            return *this;
        }

        Resource &operator*() {
            assert(!m_weak);
            return *(m_pool->get(m_index));
        }

        Resource *operator->() {
            assert(!m_weak);
            return m_pool->get(m_index);
        }

        std::size_t getRefCount() const {
            assert(!m_weak);
            return m_pool->getCount(m_index);
        }

        void destroy()
        {
            decrement();
            m_weak = true;
            m_pool.reset();
        }

    private:
        inline void increment() {
            if (!m_weak) {
                m_pool->incrementCounter(m_index);
            }
        }

        inline void decrement() {
            if (!m_weak) {
                m_pool->decrementCounter(m_index);
            }
        }

        bool m_weak{true};
        ResourceIndex m_index{0};
        PoolPtr m_pool{nullptr};
    };

}

#endif //PE_RES_RESOURCEHANDLE_H
