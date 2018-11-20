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

#include "ResourcePool.h"
#include "ResourceHandle.h"

#include "PE/Utils/Utils.h"

namespace PE::Resource {

    class ResourceManager;


    /**
     * Resource manager
     */
    class ResourceManager {
        using PoolPtr = std::shared_ptr<BaseResourcePool>;

    public:
        ResourceManager() = default;

        virtual ~ResourceManager() = default;

        template<typename Resource, typename ... Args>
        ResourceHandle<Resource> create(Args &&... args);

        template<typename Resource, typename ... Args>
        ResourceHandle<Resource> load(const std::string &path, Args &&... args);

        template<class Resource>
        Resource *get(ResourceIndex);

        template<typename Resource>
        std::size_t getSize() const;
    private:
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


        template<typename Resource>
        void createPool();

        std::vector<PoolPtr> m_pools;
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

        m_pools.emplace(m_pools.begin() + index, new ResourcePool<Resource>());
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
        auto index = ResourceCounter<Resource>::getFamily();

        if (index >= m_pools.size()) {
            createPool<Resource>();
        }

        return static_cast<ResourcePool<Resource> *>(m_pools[index].get())
                ->createResource(std::forward<Args>(args) ...);
    }

    /**
     * Load resource
     * @tparam Resource
     * @tparam Args
     * @param path
     * @param args
     * @return
     */
    template<typename Resource, typename... Args>
    ResourceHandle<Resource> ResourceManager::load(const std::string &path, Args &&... args) {
        auto index = ResourceCounter<Resource>::getFamily();

        if (index >= m_pools.size()) {
            createPool<Resource>();
        }

        return static_cast<ResourcePool<Resource> *>(m_pools[index].get())
                ->loadResource(path, std::forward<Args>(args) ...);
    }

    /**
     * Get Size
     * @tparam Resource
     * @return
     */
    template<typename Resource>
    std::size_t ResourceManager::getSize() const {
        auto index = ResourceCounter<Resource>::getFamily();

        return static_cast<ResourcePool<Resource> *>(m_pools[index].get())
                ->getSize();
    }

    /**
     * Get Resource pointer
     * @tparam Resource
     * @param resIndex
     * @return
     */
    template<class Resource>
    Resource *ResourceManager::get(ResourceIndex resIndex) {
        auto index = ResourceCounter<Resource>::getFamily();

        return static_cast<ResourcePool<Resource> *>(m_pools[index].get())
                ->get(resIndex);
    }

}

#endif //PE_RES_RESOURCEMANAGER_H
