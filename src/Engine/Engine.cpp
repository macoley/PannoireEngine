#include "PE/Engine/Engine.h"


namespace PE::Engine {


    Engine::Engine() : m_ecs(std::make_shared<ECS>()) {
        //Render::Render render;
        //render.render("Render z engina");
    }

    void Engine::init() {
        m_ecs->registerComponent<ComponentType::Render>();
        m_ecs->createSystem<RenderSystem>();
        auto entity = m_ecs->createEntity();
        auto entity2 = m_ecs->createEntity();
        entity.assignComponent<ComponentType::Transform>(.5f, .2f, .1f);
        entity2.assignComponent<ComponentType::Transform>(.5f, .2f, .1f);


        m_ecs->updateSystem<RenderSystem>();

        const auto &component = entity.getComponent<ComponentType::Transform>();

        std::cout << component.x << "\n"
                  << component.y << "\n"
                  << component.z << "\n";
    }



}