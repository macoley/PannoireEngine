#ifndef PANNOIREENGINE_ECS_UTILS_H
#define PANNOIREENGINE_ECS_UTILS_H

#include "PE/ECS/Common.h"
#include "PE/ECS/Component.h"

namespace PE::ECS {

    template<typename C>
    class Component;

    // Get Component
    template<typename C>
    FamilyIndex getComponentFamily() {
        return Component<typename std::remove_const<C>::type>::getFamily();
    }

    // Get Mask from Template
    template <typename Single>
    ComponentMask getMask()
    {
        return ComponentMask().set(getComponentFamily<Single>());
    };

    // * must be first and second for overloading
    template <typename First, typename Second, typename ... Rest>
    ComponentMask getMask()
    {
        return getMask<First>() |= getMask<Second, Rest ...>();
    };

}

#endif //PANNOIREENGINE_ECS_UTILS_H
