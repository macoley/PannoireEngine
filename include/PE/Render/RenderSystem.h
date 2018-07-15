#ifndef PANNOIREENGINE_RENDERSYSTEM_H
#define PANNOIREENGINE_RENDERSYSTEM_H

#include "PE/Engine/System.h"
#include "PE/Engine/ComponentType.h"

namespace PE::Render {
    namespace Component = PE::Engine::ComponentType;

    class RenderSystem
            : public PE::Engine::System<const Component::Transform, const Component::Render> {
    public:
        void update(const PE::Engine::EntitySet &, const Component::Transform *, const Component::Render *) final;

    };
}


#endif //PANNOIREENGINE_RENDERSYSTEM_H
