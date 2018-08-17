#ifndef PANNOIREENGINE_RESOURCEHANDLE_H
#define PANNOIREENGINE_RESOURCEHANDLE_H

#include <cstdint>

#include "ResourcePool.h"

namespace PE::Resource {

    /**
     * Resource Handle
     * @tparam Resource
     */
    template<typename Resource>
    class ResourceHandle {
        using handleType = ResourceHandle<Resource>;
        using resourcePoolType = ResourcePool<Resource>;

    public:

        /**
        * Reference Counter
        */
        class RefCounter {
        public:
            RefCounter() : m_counter(1) {}

            void increase() { ++m_counter; }

            void decrease() { --m_counter; }

            uint32_t getCount() { return m_counter; }

        private:
            uint32_t m_counter;
        };


        ResourceHandle(resourcePoolType *pool, resourceIndex resource)
                : m_resource(resource), m_pool(pool), m_counter(new RefCounter) {}

        ~ResourceHandle() { decRef(); }

        ResourceHandle(const ResourceHandle<Resource> &obj) {
            addRef(obj);
        }

        handleType &operator=(handleType &obj) {
            if (this != &obj) {
                if (m_resource != obj.m_resource) {
                    decRef();
                    addRef(obj);
                }
            }

            return *this;
        }

        void decRef() {
            m_counter->decrease();

            if (m_counter->getCount() == 0) {
                delete m_counter;
                release();
            }
        }

        void addRef(const handleType &obj) {
            m_counter = obj.m_counter;
            m_counter->increase();
            m_resource = obj.m_resource;
        }


        Resource &get() {
            return m_pool->get(m_resource);
        }

    private:
        void release() {
            m_pool->destroyResource(m_resource);
        }

        resourceIndex m_resource;
        RefCounter *m_counter;
        resourcePoolType *m_pool;
    };

}

#endif //PANNOIREENGINE_RESOURCEHANDLE_H
