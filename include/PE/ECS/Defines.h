#ifndef PE_ECS_DEFINES_H
#define PE_ECS_DEFINES_H

#include <cstdint>

namespace PE::ECS {

    // Entity ID type
    using Entity          = uint64_t;
    using EntityIndex     = uint32_t;
    using EntityVersion   = uint32_t;

    using ComponentFamily = std::size_t;

}

#endif //PE_ECS_DEFINES_H
