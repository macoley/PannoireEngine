#ifndef PE_RES_RESOURCEMANAGER_H
#define PE_RES_RESOURCEMANAGER_H

#include <vector>
#include <string>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <type_traits>
#include <memory>
#include <typeindex>

#include "ResourcePool.h"
#include "ResourceHandle.h"
#include "ResourceCounter.h"

#include "PE/Utils/Utils.h"
#include "PE/Utils/Listener.h"
#include "PE/Utils/ListenerBus.h"

namespace PE::Resource {

    /**
     * Resource manager
     */
    class ResourceManager : public Utils::ListenerBus<ResourceEvents> {
        using PoolPtr = std::shared_ptr<BaseResourcePool>;

    public:
        explicit ResourceManager() = default;

        virtual ~ResourceManager() = default;

        template<typename Resource, typename ... Args>
        ResourceHandle<Resource> create(Args &&... args);

        template<typename Resource>
        ResourceHandle<Resource> load(const std::string &path);

        template<class Resource>
        Resource *get(ResourceIndex);

        template<class Resource>
        bool has(ResourceIndex) const;

        template<typename Resource>
        std::size_t getSize() const;

        template<typename Resource, typename ... Args>
        void registerResource(Args&& ... args);

    private:
        template<typename Resource, typename ... Args>
        void createPool(Args&& ... args);

        //std::vector<PoolPtr> m_pools{};

        template <typename Resource>
        inline ResourcePool<Resource>* getPoolPtr() {
            return static_cast<ResourcePool<Resource>*>(m_pools.at(std::type_index(typeid(Resource))).get());
        }

        template <typename Resource>
        inline const ResourcePool<Resource>* getPoolPtr() const {
            return static_cast<const ResourcePool<Resource>*>(m_pools.at(std::type_index(typeid(Resource))).get());
        }

        std::unordered_map<std::type_index, PoolPtr> m_pools{};
    };


    /**
     * Create Resource Pool
     * @tparam Resource
     */
    template<typename Resource, typename ... Args>
    void ResourceManager::createPool(Args&& ... args) {
        auto index = Resource::id();

        std::cout << "Creating pool for: " << std::to_string(index) << " " << typeid(Resource).name() << std::endl;

        /*
        if (index >= m_pools.size()) {
            m_pools.resize(index);
        }
         */

        using PoolType = ResourcePool<Resource>;
        auto pool = new PoolType(std::forward<Args>(args)...);

        m_pools.emplace(std::type_index(typeid(Resource)), pool);

        addSubscriber(std::bind(&PoolType::onMsg, pool, std::placeholders::_1, std::placeholders::_2));
    }

    /**
     * Create Resource
     * @tparam Resource
     * @tparam Args
     * @param args
     * @return
     */
    template<typename Resource, typename... Args>
    ResourceHandle<Resource> ResourceManager::create(Args &&... args) {
        //assert(Resource::id() < m_pools.size());

        return getPoolPtr<Resource>()->createResource(std::forward<Args>(args)...);
    }

    /**
     * Load resource
     * @tparam Resource
     * @tparam Args
     * @param path
     * @param args
     * @return
     */
    template<typename Resource>
    ResourceHandle<Resource> ResourceManager::load(const std::string &path) {

        //assert(Resource::id() < m_pools.size());

        return getPoolPtr<Resource>()->loadResource(path);
    }

    /**
     * Get Size
     * @tparam Resource
     * @return
     */
    template<typename Resource>
    std::size_t ResourceManager::getSize() const {
        //assert(Resource::id() < m_pools.size());

        return getPoolPtr<Resource>()->getSize();
    }

    /**
     * Get Resource pointer
     * @tparam Resource
     * @param resIndex
     * @return
     */
    template<class Resource>
    Resource *ResourceManager::get(ResourceIndex resIndex) {
        //assert(Resource::id() < m_pools.size());

        return getPoolPtr<Resource>()->get(resIndex);
    }

    /**
     * Has resource?
     * @tparam Resource
     * @return
     */
    template<class Resource>
    bool ResourceManager::has(ResourceIndex resIndex) const {
        //assert(Resource::id() < m_pools.size());

        return getPoolPtr<Resource>()->has(resIndex);
    }

    /**
     *
     * @tparam Resource
     * @tparam Args
     * @param args
     */
    template<typename Resource, typename... Args>
    void ResourceManager::registerResource(Args &&... args) {
        //assert(Resource::id() >= m_pools.size()); // the resource doesn't exist in manager

        createPool<Resource, Args...>(std::forward<Args>(args)...);
    }

}

#endif //PE_RES_RESOURCEMANAGER_H
