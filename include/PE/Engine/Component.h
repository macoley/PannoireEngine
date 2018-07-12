#ifndef PANNOIREENGINE_COMPONENT_H
#define PANNOIREENGINE_COMPONENT_H

#include "PE/Engine/ECS.h"

namespace PE::Engine {

    class ECS;

    /*
     * Base Component for template indexing
     */
    class BaseComponent {
    public:
        typedef std::size_t FamilyIndex;

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
        // todo wrappers on ECS?

    private:
        static FamilyIndex getFamily();
    };

    /*
     * Family index getter
     */
    template<typename C>
    BaseComponent::FamilyIndex Component<C>::getFamily() {
        static FamilyIndex family = m_family_counter++;
        return family;
    }
}

#endif //PANNOIREENGINE_COMPONENT_H