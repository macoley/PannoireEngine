#ifndef PE_ECS_COMPONENT_H
#define PE_ECS_COMPONENT_H

#include "Defines.h"

namespace PE::ECS {

    /*
     * Base Engine for template indexing
     */
    struct BaseComponent {
    protected:
        static ComponentFamily m_family_counter;
    };

    /**
     * Component Class
     */
    template<typename C>
    struct Component : public BaseComponent {
        static ComponentFamily getFamily();
    };

    /*
     * Family index getter
     */
    template<typename C>
    ComponentFamily Component<C>::getFamily() {
        static ComponentFamily family = m_family_counter++;
        return family;
    }

}

#endif //PE_ECS_COMPONENT_H
