#include "PE/Render/RenderSystem.h"

#include <iostream>

namespace PE::Render {
    void RenderSystem::update(const PE::Engine::EntitySet & entities, const Component::Transform * t, const Component::Render * r) {

        for(const auto & e : entities)
        {
            m_context->render();
        }
    }
}