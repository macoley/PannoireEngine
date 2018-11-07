#ifndef PE_ECS_MANAGER_H
#define PE_ECS_MANAGER_H

#include <limits>
#include <vector>
#include <memory>

#include "PE/Utils/Utils.h"

#include "Defines.h"
#include "Entity.h"
#include "View.h"
#include "Component.h"
#include "ComponentSet.h"
//#include "Pool.h"

namespace PE::ECS {

    template <typename ... Component>
    class View;

    /**
     * Main manager of ECS System
     */
    class Manager
            : public std::enable_shared_from_this<Manager>
    {
        using ComponentMask   = uint32_t;
        using BaseSetPtr      = std::shared_ptr<BaseSet>;

    public:

        /**
         * Entities
         */
        const Entity createEntity();
        void destroyEntity(Entity entity);

        /**
         * Components
         */
        template <typename C, typename ... Args>
        void assignComponent(Entity, Args && ...);

        template<typename C>
        void removeComponent(Entity);

        template<typename C>
        C& getComponent(Entity);

        /**
         * Views
         */

        template <typename ... C>
        View<C...> view();

    private:

        /**
         * Entities
         */

        void accomodateEntity(EntityIndex);

        inline bool validEntity(EntityIndex index, EntityVersion version) const {
            return  (index < m_entity_version.size()) &&
                    (version == m_entity_version[index]);
        }

        /**
         * Components
         */
        inline void removeComponent(Entity, ComponentFamily);

        /**
         * Data
         */
        EntityIndex m_entity_index_counter = 0;                  // Index counter
        std::vector<EntityIndex> m_free_entity_list;             // List of free indexes

        std::vector<EntityVersion> m_entity_version;             // Versions of each entity
        std::vector<ComponentMask> m_entity_component_mask;      // Mask of components for each entity

        std::vector<BaseSetPtr> m_component_pools;               // Memory pools for each component
    };


    /**
     *  Removing Component
     */
    void Manager::removeComponent(Entity entity, ComponentFamily family) {
        const auto index = getIndex(entity);

        assert(validEntity(index, getVersion(entity)));

        m_entity_component_mask[index] &= ~(ComponentMask(1) << family);
        m_component_pools[family]->destroy(entity);
    }

    template<typename C>
    void Manager::removeComponent(Entity entity) {
        const auto family = Component<C>::getFamily();
        removeComponent(entity, family);
    }

    template<typename C, typename... Args>
    void Manager::assignComponent(Entity entity, Args &&... args) {
        const auto index = getIndex(entity);
        const auto version = getVersion(entity);
        const auto family = Component<C>::getFamily();

        assert(validEntity(index, version));

        if(family >= m_component_pools.size())
        {
            m_component_pools.emplace_back(new ComponentSet<C>);

            assert(m_component_pools.size()-1 == family); // if components were created only here?
        }

        std::static_pointer_cast<ComponentSet<C>>(m_component_pools[family])
                ->add(entity, std::forward<Args>(args)...);

        Utils::log(std::to_string(family) + " Component added to " + std::to_string(index) + " Entity.");
    }

    template<typename... C>
    View<C ...> Manager::view() {
        return View<C...>(
                std::static_pointer_cast<ComponentSet<C>>(m_component_pools[Component<C>::getFamily()]) ...
        );
    }

    /**
     * Get Component
     * @tparam C
     * @return
     */
    template<typename C>
    C &Manager::getComponent(Entity entity) {
        const auto index = getIndex(entity);
        const auto version = getVersion(entity);
        const auto family = Component<C>::getFamily();

        assert(validEntity(index, version));

        return std::static_pointer_cast<ComponentSet<C>>(m_component_pools[family])
                ->get(entity);
    }

}

#endif //PE_ECS_MANAGER_H
