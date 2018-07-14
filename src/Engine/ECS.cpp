
#include "PE/Engine/ECS.h"

namespace PE::Engine {

    /*
     * Entities
     */

    // Creating Entity
    Entity ECS::createEntity() {
        uint32_t index, version;

        if (m_free_entity_list.empty()) {
            index = m_entity_index_counter++;

            accomodateEntity(index);
            version = m_entity_version[index] = 1;
        } else {
            index = m_free_entity_list.back();
            m_free_entity_list.pop_back();
            version = m_entity_version[index];
        }

        return Entity(shared_from_this(), Entity::ID(index, version));
    }

    // Destroying Entity
    void ECS::destroyEntity(Entity::ID t_id) {
        const uint32_t index = t_id.index();
        auto mask = m_entity_component_mask[index];

        // remove each component for this entity
        for (std::size_t family = 0; family < m_component_pools.size(); ++family) {
            if (mask.test(family)) {
                removeComponent(t_id, family);
            }
        }

        m_entity_component_mask[index].reset();
        m_entity_version[index]++;
        m_free_entity_list.push_back(index);
    }

    // Creating space for Entity
    void ECS::accomodateEntity(uint32_t t_index) {
        if (m_entity_component_mask.size() <= t_index) {
            m_entity_component_mask.resize(t_index + 1);
            m_entity_version.resize(t_index + 1);

            for (auto &pool : m_component_pools) {
                pool->expand(t_index + 1);
            }
        }
    }

    Entity ECS::getEntity(Entity::ID t_id) {
        return Entity(shared_from_this(), t_id);
    }

    /*
     * Components
     */

    void ECS::removeComponent(Entity::ID t_id, FamilyIndex t_family) {
        const auto index = t_id.index();
        const auto originalMask = m_entity_component_mask[t_id.index()];

        // Remove component
        m_entity_component_mask[index].reset(t_family);
        m_component_pools[t_family]->destroy(index);

        // Remove from system
        for (std::size_t system_index = 0; system_index < m_system_component_mask.size(); ++system_index) {
            if(
                    (originalMask & m_system_component_mask[system_index]) == m_system_component_mask[system_index] &&
                    (m_entity_component_mask[index] & m_system_component_mask[system_index]) != m_system_component_mask[system_index]
            )
            {
                m_systems[system_index]->removeEntity(index);
            }
        }
    }
}