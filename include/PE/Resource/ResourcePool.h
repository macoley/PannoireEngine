#ifndef PE_RES_RESOURCEPOOL_H
#define PE_RES_RESOURCEPOOL_H

#include <vector>
#include <cassert>
#include <stack>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <any>

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

    protected:
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
        template<typename ... Dependencies>
        explicit ResourcePool(Dependencies&& ... deps)
            : m_prototype(new Resource(std::forward<Dependencies>(deps)...)) {}

        virtual ~ResourcePool() = default;

        /*
         * Resource Managment
         */

        template<typename ... Args>
        Handle createResource(Args&& ... args);

        Handle loadResource(const std::string &path);

        bool has(ResourceIndex index) const;

        Resource *get(ResourceIndex);

        /*
         * Others
         */

        void incrementCounter(ResourceIndex);

        void decrementCounter(ResourceIndex);

        std::size_t getCount(ResourceIndex) const;

        std::size_t getSize() const;

        /*
         * Message system
         */

        void onMsg(ResourceEvents, const std::any&);

    protected:

        /*
         * Private Resource Managment
         */

        ResourceIndex allocResource(Resource*);

        void destroyResource(ResourceIndex);

        void swapLoadedResource(const std::string &old_path, const std::string &new_path);

        /*
         * Helpers
         */

        inline ResourceIndex getResID(ResourceIndex proxy_index) const {
            return m_proxy[proxy_index];
        }

        inline ReferenceCount* getResCounter(ResourceIndex proxy_index) {
            return m_resources[getResID(proxy_index)].second.get();
        }

        inline Resource* getRes(ResourceIndex proxy_index) {
            return m_resources[getResID(proxy_index)].first.get();
        }

        /*
         * Data
         */
        std::unordered_map<std::string, ResourceIndex> m_cache;
        std::unordered_map<ResourceIndex, std::string> m_reverse_cache;

        std::vector<std::pair<ResourcePtr, CounterPtr>> m_resources;

        std::vector<ResourceIndex> m_proxy;
        std::vector<ResourceIndex> m_proxy_reverse;
        std::stack<ResourceIndex> m_proxy_free_indexes;

        std::vector<uint32_t> m_aliases_counter;

        // Prototype
        ResourcePtr m_prototype;
    };


    /**
     * Create resource
     * @tparam Resource
     * @tparam Dependencies
     * @return
     */
    template<typename Resource>
    template<typename ... Args>
    typename ResourcePool<Resource>::Handle ResourcePool<Resource>::createResource(Args&& ... args) {
        return {allocResource(new Resource(std::forward<Args>(args)...)), this->shared_from_this()};
    }

    /**
     * Alloc resource
     * @tparam Resource
     * @tparam Dependencies
     * @return
     */
    template<typename Resource>
    ResourceIndex ResourcePool<Resource>::allocResource(Resource * res) {
        ResourceIndex proxy_index, res_index;

        if (m_proxy_free_indexes.empty()) {
            proxy_index = m_proxy.size();
            res_index = m_resources.size();
            m_proxy.push_back(res_index);

            if (res_index >= m_aliases_counter.size()) {
                m_aliases_counter.resize(res_index + 1, 0);
            }
        } else {
            proxy_index = m_proxy_free_indexes.top();
            m_proxy_free_indexes.pop();
            m_proxy[proxy_index] = m_resources.size();
        }

        m_resources.emplace_back(res, new ReferenceCount());
        m_proxy_reverse.push_back(proxy_index);

        return proxy_index;
    }

    /**
     * Load resource
     * @tparam Resource
     * @tparam Dependencies
     * @param path
     * @return
     */
    template<typename Resource>
    typename ResourcePool<Resource>::Handle ResourcePool<Resource>::loadResource(const std::string &path) {
        static_assert(std::is_base_of<IResource<Resource>, Resource>::value, "Resource must be inherited from LoadableResource");

        auto search = m_cache.find(path);

        if (search != m_cache.end()) {
            // found
            Utils::log("Res from cache: " + path + " [" + typeid(Resource).name() + "]");

            return {search->second, this->shared_from_this()};
        } else {
            // not found
            Utils::log("Res from file: " + path + " [" + typeid(Resource).name() + "]");

            auto proxy_index = allocResource(new Resource(*m_prototype));

            // add to cache
            m_cache[path] = proxy_index;
            m_reverse_cache[proxy_index] = path;

            getRes(proxy_index)->load(path);

            return {proxy_index, this->shared_from_this()};
        }
    }

    /**
     * Swap loaded resource
     * @tparam Resource
     * @tparam Dependencies
     * @param old_path
     * @param new_path
     */
    template<typename Resource>
    void ResourcePool<Resource>::swapLoadedResource(const std::string &old_path,
                                                                     const std::string &new_path) {
        assert((m_cache.find(old_path) != m_cache.end()));

        auto proxy_index = m_cache[old_path];

        if ((m_cache.find(new_path) != m_cache.end()) && (old_path != new_path)) {
            // found
            m_cache.erase(old_path);
            m_reverse_cache[proxy_index] = new_path;

            m_proxy[proxy_index] = m_proxy[m_cache[new_path]];
            m_aliases_counter[proxy_index]++;

        } else {
            // not found
            ResourcePtr newRes(new Resource(*m_prototype));
            newRes->load(new_path);

            m_resources[getResID(proxy_index)].first.swap(newRes);

            m_cache.erase(old_path);
            m_cache[new_path] = proxy_index;
            m_reverse_cache[proxy_index] = new_path;
        }

    }

    /**
     * Has resource?
     * @tparam Resource
     * @tparam Dependencies
     * @param index
     * @return
     */
    template<typename Resource>
    bool ResourcePool<Resource>::has(ResourceIndex index) const {
        return getCount(index) > 0;
    }

    /**
     * Get resource
     * @tparam Resource
     * @tparam Dependencies
     * @return
     */
    template<typename Resource>
    Resource *ResourcePool<Resource>::get(ResourceIndex proxy_index) {
        return getRes(proxy_index);
    }

    /**
     * Inc counter
     * @tparam Resource
     * @tparam Dependencies
     */
    template<typename Resource>
    void ResourcePool<Resource>::incrementCounter(ResourceIndex proxy_index) {
        getResCounter(proxy_index)->increment();
    }

    /**
     * Decrement counter
     * @tparam Resource
     * @tparam Dependencies
     * @param proxy_index
     */
    template<typename Resource>
    void ResourcePool<Resource>::decrementCounter(ResourceIndex proxy_index) {
        getResCounter(proxy_index)->decrement();

        if(getResCounter(proxy_index)->getCount() == 0)
            destroyResource(proxy_index);
    }

    /**
     * Get count
     * @tparam Resource
     * @tparam Dependencies
     * @return
     */
    template<typename Resource>
    std::size_t ResourcePool<Resource>::getCount(ResourceIndex proxy_index) const {
        return m_resources[getResID(proxy_index)].second->getCount();
    }

    /**
     * Get size
     * @tparam Resource
     * @tparam Dependencies
     * @return
     */
    template<typename Resource>
    std::size_t ResourcePool<Resource>::getSize() const {
        return m_resources.size();
    }

    /**
     * Destroy resource
     * @tparam Resource
     * @tparam Dependencies
     */
    template<typename Resource>
    void ResourcePool<Resource>::destroyResource(ResourceIndex proxy_index) {
        auto proxy_reverse_element_index = m_proxy[proxy_index];

        if(m_aliases_counter[proxy_reverse_element_index] > 0)
        {
            m_aliases_counter[proxy_reverse_element_index]--;
            m_proxy[proxy_index] = INVALID_INDEX;
            m_reverse_cache.erase(proxy_index);
        }
        else
        {
            auto proxy_last_index = m_proxy_reverse.back();

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
    }

    /**
     * On message [message system]
     * @tparam Resource
     * @tparam Dependencies
     */
    template<typename Resource>
    void ResourcePool<Resource>::onMsg(ResourceEvents event, const std::any & data) {
        switch (event) {
            case ResourceEvents::FILE_CHANGED:
                auto fileInfo = std::any_cast<FileInfo>(data);

                if((m_cache.find(fileInfo.m_path) != m_cache.end())) // if in cache
                {
                    swapLoadedResource(fileInfo.m_path, fileInfo.m_path);
                }

                break;
        }
    }



}

#endif //PE_RES_RESOURCEPOOL_H
