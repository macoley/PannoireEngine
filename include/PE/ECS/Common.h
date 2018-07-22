#ifndef PANNOIREENGINE_COMMON_H
#define PANNOIREENGINE_COMMON_H

#include <bitset>
#include <set>

namespace PE::ECS {

    const static std::size_t MAX_COMPONENTS = 64;   // Max components amount

    /*
     * Typedefs
     */
    typedef std::size_t FamilyIndex;
    typedef std::size_t SystemIndex;
    typedef std::bitset<MAX_COMPONENTS> ComponentMask;
    typedef std::set<std::uint32_t> EntitySet;

}

#endif //PANNOIREENGINE_COMMON_H
