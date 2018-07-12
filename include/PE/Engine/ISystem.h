#ifndef PANNOIREENGINE_ISYSTEM_H
#define PANNOIREENGINE_ISYSTEM_H

#include <vector>
#include <cstdint>
#include <algorithm>

#include "PE/Engine/ECS.h"

class ECS;

/**
 * Interface for systems
 * @tparam Cargs
 */
template <typename ... Cargs>
class ISystem {

    virtual void update(Cargs * ...) = 0;

protected:
    std::vector<uint32_t> m_entities;

private:
    friend class ECS;

    inline void addEntity(uint32_t index) {
        m_entities.push_back(index);
        std::sort(m_entities.begin(), m_entities.end());
    }

    inline void removeEntity(uint32_t index) {
        m_entities.push_back(index);
        std::sort(m_entities.begin(), m_entities.end());
    }
};

#endif //PANNOIREENGINE_ISYSTEM_H
