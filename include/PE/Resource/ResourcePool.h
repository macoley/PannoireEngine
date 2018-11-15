#ifndef PE_RES_RESOURCEPOOL_H
#define PE_RES_RESOURCEPOOL_H

#include <vector>
#include <cassert>
#include <stack>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#include "Defines.h"
#include "IResource.h"
#include "ResourceHandle.h"

#include "PE/Utils/Utils.h"

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

        enum : ResourceIndex {
            INVALID_INDEX = ~ResourceIndex(0)
        };

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

        inline ResourceIndex getResID(ResourceIndex proxy_index) const {
            return m_proxy[proxy_index];
        }

        inline ReferenceCount* getResCounter(ResourceIndex proxy_index) {
            return m_resources[getResID(proxy_index)].second.get();
        }

        inline Resource* getRes(ResourceIndex proxy_index) {
            return m_resources[getResID(proxy_index)].first.get();
        }

        inline void print() {
            std::cout << "-------------------" << std::endl;

            std::cout << std::string(typeid(Resource).name()) << std::endl;

            std::cout << "m_proxy [size " << m_proxy.size() << "] :";
            std::for_each(m_proxy.begin(), m_proxy.end(), [](auto &e) {
                std::cout << " " << (e == INVALID_INDEX ? std::string("null") : std::to_string(e));
            });
            std::cout << std::endl;

            std::cout << "m_proxy_reverse [size " << m_proxy_reverse.size() << "] :";
            std::for_each(m_proxy_reverse.begin(), m_proxy_reverse.end(), [](auto &e) {
                std::cout << " " << e;
            });
            std::cout << std::endl;

            std::cout << "m_resources [size " << m_resources.size() << "] :";
            std::for_each(m_resources.begin(), m_resources.end(), [](auto &e) {
                std::cout << " [" << (e.first.get() == nullptr ? "null": "res") << " - " << e.second.get()->getCount() << "]";
            });
            std::cout << std::endl;

            std::cout << "-------------------" << std::endl;
        }

        std::unordered_map<std::string, ResourceIndex> m_cache;
        std::unordered_map<ResourceIndex, std::string> m_reverse_cache;

        std::vector<std::pair<ResourcePtr, CounterPtr>> m_resources;

        std::vector<ResourceIndex> m_proxy;
        std::vector<ResourceIndex> m_proxy_reverse;
        std::stack<ResourceIndex> m_proxy_free_indexes;
    };



    /**
     * Destroy resource
     * @tparam Resource
     */
    template<typename Resource>
    void ResourcePool<Resource>::destroyResource(ResourceIndex proxy_index) {

        auto proxy_last_index = m_proxy_reverse.back();
        auto proxy_reverse_element_index = m_proxy[proxy_index];

        m_proxy[proxy_last_index] = m_proxy[proxy_index];
        m_proxy[proxy_index] = INVALID_INDEX;

        std::swap(m_proxy_reverse[proxy_reverse_element_index], m_proxy_reverse.back());
        m_proxy_reverse.pop_back();

        std::swap(m_resources[proxy_reverse_element_index], m_resources.back());
        m_resources.pop_back();

        m_proxy_free_indexes.push(proxy_index);

        if (m_reverse_cache.find(proxy_index) != m_reverse_cache.end()) {
            auto path = m_reverse_cache[proxy_index];

            Utils::log("Res deleted: " + path +  " [" + typeid(Resource).name() + "]");

            m_cache.erase(path);
            m_reverse_cache.erase(proxy_index);
        }
    }

    template<typename Resource>
    template<typename... Args>
    typename ResourcePool<Resource>::Handle ResourcePool<Resource>::loadResource(const std::string &path, Args &&... args) {
        static_assert(std::is_base_of<IResource, Resource>::value, "Resource must be inherited from LoadableResource");

        auto search = m_cache.find(path);

        if (search != m_cache.end()) {
            // found
            Utils::log("Res from cache: " + path + " [" + typeid(Resource).name() + "]");

            return {search->second, this->shared_from_this()};
        } else {
            // not found
            Utils::log("Res from file: " + path + " [" + typeid(Resource).name() + "]");

            auto proxy_index = allocResource(std::forward<Args>(args)...);

            // add to cache
            m_cache[path] = proxy_index;
            m_reverse_cache[proxy_index] = path;

            getRes(proxy_index)->load(path);

            return {proxy_index, this->shared_from_this()};
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
        ResourceIndex proxy_index, res_index;

        if (m_proxy_free_indexes.empty()) {
            proxy_index = m_proxy.size();
            res_index = m_resources.size();
            m_proxy.push_back(res_index);
        } else {
            proxy_index = m_proxy_free_indexes.top();
            m_proxy_free_indexes.pop();
            m_proxy[proxy_index] = m_resources.size();
        }

        m_resources.emplace_back(new Resource(std::forward<Args>(args)...), new ReferenceCount());
        m_proxy_reverse.push_back(proxy_index);

        return proxy_index;
    }

    /**
     * Increment resource
     * @tparam Resource
     * @param index
     */
    template<typename Resource>
    void ResourcePool<Resource>::incrementCounter(ResourceIndex proxy_index) {
        getResCounter(proxy_index)->increment();
    }

    /**
     * Decrement resource
     * @tparam Resource
     * @param index
     */
    template<typename Resource>
    void ResourcePool<Resource>::decrementCounter(ResourceIndex proxy_index) {
        getResCounter(proxy_index)->decrement();

        if(getResCounter(proxy_index)->getCount() == 0)
            destroyResource(proxy_index);
    }

    /**
     * Get resource
     * @tparam Resource
     * @param index
     * @return
     */
    template<typename Resource>
    Resource *ResourcePool<Resource>::get(ResourceIndex proxy_index) {
        return getRes(proxy_index);
    }

    template<typename Resource>
    std::size_t ResourcePool<Resource>::getCount(ResourceIndex proxy_index) const {
        return getResCounter(proxy_index)->getCount();
    }

    template<typename Resource>
    std::size_t ResourcePool<Resource>::getSize() const {
        return m_resources.size();
    }


}

#endif //PE_RES_RESOURCEPOOL_H
