
#include "PE/ECS/ComponentSet.h"

namespace PE::ECS {

    void BaseSet::destroy(Entity entity) {
        assert(has(entity));

        auto entityIndex = getIndex(entity);

        const auto back = m_direct.back();          // entity
        auto &candidate = m_reverse[entityIndex];   // dataIndex
        m_reverse[getIndex(back)] = candidate;      // reverse[entity] = dataIndex
        m_direct[candidate] = back;                 // direct[dataIndex] = entity
        candidate = INVALID_ENTITY;
        m_direct.pop_back();
    }

    void BaseSet::add(Entity entity) {
        assert(!has(entity));

        auto entityIndex = getIndex(entity);

        if (entityIndex >= m_reverse.size()) {
            m_reverse.resize(entityIndex + 1, INVALID_ENTITY);
        }

        m_reverse[entityIndex] = VALID_ENTITY;
        m_direct.push_back(entity);
    }

}