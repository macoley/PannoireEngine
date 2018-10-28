#ifndef PANNOIREENGINE_COMPONENT_H
#define PANNOIREENGINE_COMPONENT_H

#include "PE/ECS/Common.h"

namespace PE::ECS {

    class ECS;

    /*
     * Base Engine for template indexing
     */
    class BaseComponent {
    protected:
        static FamilyIndex m_family_counter;
    };

    /**
     * Component Class
     */
    template<typename C>
    class Component : public BaseComponent {
        friend class ECS;

    public:
        static FamilyIndex getFamily();

        // todo wrappers on ECS?
    };

    /*
     * Family index getter
     */
    template<typename C>
    FamilyIndex Component<C>::getFamily() {
        static FamilyIndex family = m_family_counter++;
        return family;
    }

}

#endif //PANNOIREENGINE_COMPONENT_H