#ifndef PE_RES_RESOURCEPOOL_H
#define PE_RES_RESOURCEPOOL_H

#include <vector>
#include <cassert>
#include <stack>
#include <memory>
#include <unordered_map>

#include "Defines.h"
#include "IResource.h"
#include "ResourceHandle.h"

namespace PE::Resource {

    template<typename Resource>
    class ResourceHandle;


    /**
     * Base Resource Pool
     */
    struct BaseResourcePool {};

    /**
     * Resource Pool
     * @tparam Resource
     */
    template<typename Resource>
    class ResourcePool
            : public BaseResourcePool,
              public std::enable_shared_from_this<ResourcePool<Resource>> {

        /**
         * Reference count
         */
        class ReferenceCount {
        public:
            void increment() {
                ++m_RefCount;
            }

            void decrement() {
                --m_RefCount;
            }

            std::size_t getCount() const {
                return m_RefCount;
            }

        private:
            std::size_t m_RefCount{0};
        };

        using ResourcePtr = std::unique_ptr<Resource>;
        using CounterPtr = std::unique_ptr<ReferenceCount>;
        using Handle = ResourceHandle<Resource>;

    public:
        explicit ResourcePool() = default;

        virtual ~ResourcePool() = default;

        template<typename... Args>
        Handle createResource(Args &&... args);

        template<typename... Args>
        Handle loadResource(const std::string &path, Args &&... args);

        void incrementCounter(ResourceIndex);

        void decrementCounter(ResourceIndex);

        Resource *get(ResourceIndex);

        std::size_t getCount(ResourceIndex) const;

        std::size_t getSize() const;

    private:
        template<typename... Args>
        ResourceIndex allocResource(Args &&... args);

        void destroyResource(ResourceIndex);

        std::unordered_map<std::string, ResourceIndex> m_cache;
        std::unordered_map<ResourceIndex, std::string> m_reverse_cache;

        std::vector<CounterPtr> m_counters; // todo std::pair
        std::vector<ResourcePtr> m_pool;
        std::stack<ResourceIndex> m_free_indexes;
        // todo proxy
    };



    /**
     * Destroy resource
     * @tparam Resource
     */
    template<typename Resource>
    void ResourcePool<Resource>::destroyResource(ResourceIndex index) {
        m_pool.erase(m_pool.begin() + index);
        m_counters.erase(m_counters.begin() + index);
        m_free_indexes.push(index);

        if (m_reverse_cache.find(index) != m_reverse_cache.end()) {
            auto path = m_reverse_cache[index];

            m_cache.erase(path);
            m_reverse_cache.erase(index);
        }
    }

    template<typename Resource>
    template<typename... Args>
    typename ResourcePool<Resource>::Handle ResourcePool<Resource>::loadResource(const std::string &path, Args &&... args) {
        static_assert(std::is_base_of<IResource, Resource>::value, "Resource must be inherited from LoadableResource");

        auto search = m_cache.find(path);

        if (search != m_cache.end()) {
            // found
            return {search->second, this->shared_from_this()};
        } else {
            // not found
            auto index = allocResource(std::forward<Args>(args)...);

            // add to cache
            m_cache[path] = index;
            m_reverse_cache[index] = path;

            m_pool[index]->load(path);

            return {index, this->shared_from_this()};
        }
    }

    template<typename Resource>
    template<typename... Args>
    typename ResourcePool<Resource>::Handle ResourcePool<Resource>::createResource(Args &&... args) {
        return {allocResource(std::forward<Args>(args)...), this->shared_from_this()};
    }

    /**
     * Alloc Resource
     * @tparam Resource
     * @tparam Args
     * @param args
     * @return
     */
    template<typename Resource>
    template<typename... Args>
    ResourceIndex ResourcePool<Resource>::allocResource(Args &&... args) {
        ResourceIndex index;

        if (m_free_indexes.empty()) {
            index = m_pool.size();
            m_pool.emplace_back(new Resource(std::forward<Args>(args)...));
            m_counters.emplace_back(new ReferenceCount());
        } else {
            index = m_free_indexes.top();
            m_free_indexes.pop();
            m_pool.emplace(m_pool.begin() + index, new Resource(std::forward<Args>(args)...));
            m_counters.emplace(m_counters.begin() + index, new ReferenceCount());
        }

        return index;
    }

    /**
     * Increment resource
     * @tparam Resource
     * @param index
     */
    template<typename Resource>
    void ResourcePool<Resource>::incrementCounter(ResourceIndex index) {
        m_counters[index]->increment();
    }

    /**
     * Decrement resource
     * @tparam Resource
     * @param index
     */
    template<typename Resource>
    void ResourcePool<Resource>::decrementCounter(ResourceIndex index) {
        m_counters[index]->decrement();

        if(m_counters[index]->getCount() == 0)
            destroyResource(index);
    }

    /**
     * Get resource
     * @tparam Resource
     * @param index
     * @return
     */
    template<typename Resource>
    Resource *ResourcePool<Resource>::get(ResourceIndex index) {
        return m_pool[index].get();
    }

    template<typename Resource>
    std::size_t ResourcePool<Resource>::getCount(ResourceIndex index) const {
        return m_counters[index]->getCount();
    }

    template<typename Resource>
    std::size_t ResourcePool<Resource>::getSize() const {
        return m_pool.size();
    }


}

#endif //PE_RES_RESOURCEPOOL_H
