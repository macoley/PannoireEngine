#ifndef PE_ECS_MANAGER_H
#define PE_ECS_MANAGER_H

#include <limits>
#include <vector>
#include <memory>

#include "Defines.h"
#include "Component.h"
#include "ComponentSet.h"
//#include "Pool.h"

namespace PE::ECS {

    /**
     * Main manager of ECS System
     */
    class Manager {
        using ComponentMask   = uint32_t;

    public:

        /**
         * Entities
         */

        Entity createEntity();
        void destroyEntity(Entity entity);

        /**
         * Components
         */
        template <typename C, typename ... Args>
        void assignComponent(Entity, Args && ...);

        template<typename C>
        void removeComponent(Entity);

    private:

        /**
         * Entities
         */

        constexpr EntityIndex indexShift()
        {
            return static_cast<EntityIndex>(std::numeric_limits<EntityIndex>::digits);
        }

        inline Entity getEntity(EntityIndex index, EntityVersion version)
        {
            return (static_cast<Entity>(index) |
                    static_cast<Entity>(version) << indexShift());
        }

        inline EntityIndex getIndex(Entity entity)
        {
            return static_cast<EntityIndex>(entity & ~EntityIndex(0));
        }

        inline EntityVersion getVersion(Entity entity)
        {
            return static_cast<EntityVersion>(entity >> indexShift());
        }

        void accomodateEntity(EntityIndex);

        /**
         * Components
         */
        inline void removeComponent(EntityIndex, ComponentFamily);

        /**
         * Data
         */
        EntityIndex m_entity_index_counter = 0;                                // Index counter
        std::vector<EntityIndex> m_free_entity_list;                           // List of free indexes

        std::vector<EntityVersion> m_entity_version;                           // Versions of each entity
        std::vector<ComponentMask> m_entity_component_mask;                    // Mask of components for each entity

        std::vector<std::unique_ptr<BaseComponentSet>> m_component_pools;      // Memory pools for each component
    };


    /**
     *  Removing Component
     */
    void Manager::removeComponent(EntityIndex index, ComponentFamily family) {
        m_entity_component_mask[index] = m_entity_component_mask[index] & ~(ComponentMask(1) << family);
        m_component_pools[family]->destroy(index);
    }

    template<typename C>
    void Manager::removeComponent(Entity entity) {
        const EntityIndex index = getIndex(entity);
        const ComponentFamily family = Component<C>::getFamily();

        removeComponent(index, family);
    }

    template<typename C, typename... Args>
    void Manager::assignComponent(Entity entity, Args &&... args) {
        const EntityIndex index = getIndex(entity);
        const ComponentFamily family = Component<C>::getFamily();

        if(family >= m_component_pools.size())
        {
            m_component_pools.emplace_back(new ComponentSet<C>);

            assert(m_component_pools.size()-1 == family); // if components were created only here?
        }

        static_cast<ComponentSet<C>*>(m_component_pools[family].get())->add(index, std::forward<Args>(args)...);

    }

}

#endif //PE_ECS_MANAGER_H
