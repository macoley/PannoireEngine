#ifndef PANNOIREENGINE_UTILS_H
#define PANNOIREENGINE_UTILS_H

#include "PE/Engine/Common.h"
#include "PE/Engine/Component.h"

namespace PE::Engine {

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

#endif //PANNOIREENGINE_UTILS_H
