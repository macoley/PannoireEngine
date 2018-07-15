#include "PE/Render/RenderSystem.h"

#include <iostream>

namespace PE::Render {
    void RenderSystem::update(const PE::Engine::EntitySet & entities, const Component::Transform * t, const Component::Render * r) {

        std::cout << "Update Renderer" << "\n";

        for(const auto & e : entities)
        {
            std::cout << "entity update" << "\n";
            std::cout << t[e].x << "\n"
                      << t[e].y << "\n"
                      << t[e].z << "\n";
        }


    }
}