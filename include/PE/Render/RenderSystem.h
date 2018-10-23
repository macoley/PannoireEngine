#ifndef PANNOIREENGINE_RENDERSYSTEM_H
#define PANNOIREENGINE_RENDERSYSTEM_H

#include <memory>

#include "PE/Engine/System.h"
#include "PE/Engine/ComponentType.h"
#include "PE/Engine/IContext.h"

namespace PE::Render {
    using namespace PE;
    namespace Component = PE::Engine::ComponentType;

    class RenderSystem
            : public Engine::System<const Component::Transform, const Component::Render> {
    public:
        // Constructor must be defined here - I don't know why.
        explicit RenderSystem(std::shared_ptr<Engine::IContext> t_context)
                : m_context(std::move(t_context))
        {}

        void update(const PE::Engine::EntitySet &, const Component::Transform *, const Component::Render *) final;

    protected:
        std::shared_ptr<Engine::IContext> m_context;
    };
}

#endif //PANNOIREENGINE_RENDERSYSTEM_H
