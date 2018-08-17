#ifndef PANNOIREENGINE_RESOURCEPOOL_H
#define PANNOIREENGINE_RESOURCEPOOL_H

#include <vector>
#include <cassert>
#include <stack>
#include <memory>

#include "Utils.h"

namespace PE::Resource {

    /**
     * Base Resource Pool
     */
    class BaseResourcePool { };

    /**
     * Resource Pool
     * @tparam Resource
     */
    template <typename Resource>
    class ResourcePool : public BaseResourcePool {
        using resourcePtr = std::unique_ptr<Resource>;

    public:
        explicit ResourcePool() : m_pool(), m_free_indexes() {};

        template<typename... Args>
        resourceIndex createResource(Args && ... args);

        void destroyResource(resourceIndex);

        Resource& get(resourceIndex);

    private:
        std::vector<resourcePtr>     m_pool;
        std::stack<resourceIndex>    m_free_indexes;
    };

    /**
     * Create Resource
     * @tparam Resource
     * @tparam Args
     * @param args
     * @return
     */
    template<typename Resource>
    template<typename... Args>
    resourceIndex ResourcePool<Resource>::createResource(Args &&... args) {
        resourceIndex index = INVALID_INDEX;

        auto resource = new Resource(std::forward<Args>(args)...);

        if(m_free_indexes.empty())
        {
            index = m_pool.size();
            m_pool.emplace_back(resource);
        }
        else
        {
            index = m_free_indexes.top();
            m_free_indexes.pop();
            m_pool.emplace(m_pool.begin() + index, resource);
        }

        return index;
    }

    /**
     * Destroy resource
     * @tparam Resource
     */
    template<typename Resource>
    void ResourcePool<Resource>::destroyResource(resourceIndex index) {
        m_pool.erase(m_pool.begin() + index);
        m_free_indexes.push(index);
    }

    /**
     * Get resource ref
     * @tparam Resource
     * @param index
     * @return
     */
    template<typename Resource>
    Resource &ResourcePool<Resource>::get(resourceIndex index) {
        return *(m_pool[index]);
    }

}

#endif //PANNOIREENGINE_RESOURCEPOOL_H
