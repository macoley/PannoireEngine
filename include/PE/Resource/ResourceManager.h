#ifndef PANNOIREENGINE_RESOURCEMANAGER_H
#define PANNOIREENGINE_RESOURCEMANAGER_H

#include <vector>
#include <string>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <type_traits>
#include <memory>

#include "PE/Resource/Utils.h"
#include "PE/Resource/ResourcePool.h"
#include "PE/Resource/ResourceHandle.h"

namespace PE::Resource {

    /**
     * Resource manager
     */
    class ResourceManager {
        using PoolPtr = std::unique_ptr<BaseResourcePool>;

    public:
        ResourceManager() = default;

        virtual ~ResourceManager() = default;

        template<typename Resource, typename ... Args>
        ResourceHandle<Resource> load(Args &&... args);

    private:
        template<typename Resource>
        void createPool();

        std::vector<PoolPtr> m_pools;

        /**
         * Base Resource Counter
         */
        class BaseResourceCounter {
        public:
            using FamilyIndex = std::size_t;

        protected:
            static FamilyIndex m_family_counter;
        };

        /**
         * Resource Counter
         */
        template<typename Resource>
        class ResourceCounter : public BaseResourceCounter {
        public:
            static FamilyIndex getFamily();
        };
    };


    /**
     *
     * @tparam Resource
     * @return
     */
    template<typename Resource>
    ResourceManager::BaseResourceCounter::FamilyIndex ResourceManager::ResourceCounter<Resource>::getFamily() {
        static FamilyIndex family = m_family_counter++;
        return family;
    }

    /**
     * Create Resource Pool
     * @tparam Resource
     */
    template<typename Resource>
    void ResourceManager::createPool() {
        auto index = ResourceCounter<Resource>::getFamily();

        if (index >= m_pools.size()) {
            m_pools.resize(index);
        }

        m_pools.emplace(m_pools.begin() + index, static_cast<BaseResourcePool *>(new ResourcePool<Resource>()));
    }

    /**
     * Load
     * @tparam Resource
     * @tparam Args
     * @param args
     * @return
     */
    template<typename Resource, typename ... Args>
    ResourceHandle<Resource> ResourceManager::load(Args &&... args) {
        auto index = ResourceCounter<Resource>::getFamily();

        if (index >= m_pools.size()) {
            createPool<Resource>();
        }

        auto pool = static_cast<ResourcePool<Resource> *>(m_pools[index].get());
        auto resourceIndex = pool->createResource(std::forward<Args>(args)...);

        return {pool, resourceIndex};
    }


}

#endif //PANNOIREENGINE_RESOURCEMANAGER_H
