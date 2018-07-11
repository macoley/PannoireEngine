#include "PE/Engine/Engine.h"


namespace PE::Engine {

    Engine::Engine() : m_ecs(std::make_shared<ECS>()) {
        //Render::Render render;
        //render.render("Render z engina");
    }

    void Engine::init() {
        m_ecs->registerComponent<ComponentType::Transform>();
        auto entity = m_ecs->createEntity();
        entity.destroyEntity();

        auto entity2 = m_ecs->createEntity();
        auto c = entity2.assignComponent<ComponentType::Transform>(.1f, .0f, .5f);
        auto raw = c.get();

        raw.x = 2;

        std::cout << raw.x << "\n"
                  << raw.y << "\n"
                  << raw.z << "\n";

        c = entity2.getComponent<ComponentType::Transform>();
        raw = c.get();

        std::cout << raw.x << "\n"
                  << raw.y << "\n"
                  << raw.z << "\n";

        entity2.removeComponent<ComponentType::Transform>();
        //auto raw2 = c.get();
    }



}