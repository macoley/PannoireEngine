#include "PE/Engine/ECS.h"

namespace PE::Engine {

    BaseComponent::Family BaseComponent::m_family_counter = 0;

    /*
     * ENTITY
     */

    /*
     * ECS
     */

    EntityHandler ECS::createEntity() {
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

        return EntityHandler(shared_from_this(), ID(index, version));
    }

    void ECS::destroyEntity(ID t_id) {
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

    void ECS::removeComponent(ID t_id, BaseComponent::Family t_family) {
        const uint32_t index = t_id.index();

        m_entity_component_mask[index].reset(t_family);

        auto pool = m_component_pools[t_family];
        pool->destroy(index);
    }

    void ECS::accomodateEntity(uint32_t t_index) {
        if (m_entity_component_mask.size() <= t_index) {
            m_entity_component_mask.resize(t_index + 1);
            m_entity_version.resize(t_index + 1);

            for (auto &pool : m_component_pools) {
                pool->expand(t_index + 1);
            }
        }
    }

}