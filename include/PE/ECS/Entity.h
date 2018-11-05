#ifndef PE_ECS_ENTRY_H
#define PE_ECS_ENTRY_H

#include <limits>
#include "Defines.h"

namespace PE::ECS {

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

}

#endif //PE_ECS_ENTRY_H
