#ifndef PANNOIREENGINE_SYSTEM_H
#define PANNOIREENGINE_SYSTEM_H

#include <vector>
#include <cstdint>
#include <algorithm>
#include <tuple>
#include <functional>
#include <memory>

#include "PE/Engine/Common.h"
#include "PE/Engine/Utils.h"

namespace PE::Engine {

    class ECS;

    /**
     * Base system
     */
    class BaseSystem {
        friend class ECS;

    protected:
        BaseSystem(ComponentMask t_mask) : m_component_mask(t_mask) {}
        static SystemIndex m_system_counter;

    private:
        ComponentMask m_component_mask;
    };

    /**
     * Interface for systems
     * @tparam Cargs
     */
    template <typename ... Components>
    class System : public BaseSystem {
        friend class ECS;

    public:
        virtual void update(const EntitySet &, Components * ...) = 0;

    protected:
        System() : BaseSystem(getMask<Components ...>()) {}

    private:
        static SystemIndex getSystemIndex();
    };


    /*
     * System index getter
     */
    template <typename ... Components>
    SystemIndex System<Components ...>::getSystemIndex() {
        static SystemIndex system_index = m_system_counter++;
        return system_index;
    }

}

#endif //PANNOIREENGINE_SYSTEM_H
