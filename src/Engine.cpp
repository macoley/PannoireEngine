#include "Engine.h"

#include <memory>
#include <stb_image.h>

namespace PE {

    Engine::Engine()
            : m_ecs(new ECS::ECS),
              m_res_manager(new Resource::ResourceManager)
    {}

    void Engine::init() {
        // UTILS
        Utils::Locator::provide(new Utils::ConsoleLogger());

        // RENDERER SYSTEM
        Render::init();
        m_context = std::shared_ptr<Render::Context>(Render::createContext());

        // RESOURCE MANAGER
        auto texture = m_res_manager->load<Render::Texture>("container.jpg");
        auto shader = m_res_manager->load<Render::Shader>("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

        m_ecs->registerComponent<Component::Transform>();
        //m_ecs->registerComponent<Component::Render>();

        /*
        m_ecs->registerComponent<ComponentType::Transform>();
        m_ecs->registerComponent<ComponentType::Render>();

        m_ecs->createSystem<Render::RenderSystem>(m_context); // todo inject Render::RenderSystem

        auto entity = m_ecs->createEntity();
        auto entity2 = m_ecs->createEntity();
        entity.assignComponent<ComponentType::Transform>(.5f, .2f, .1f);
        entity.assignComponent<ComponentType::Render>(.5f);
        entity2.assignComponent<ComponentType::Transform>(.5f, .2f, .1f);
        entity2.assignComponent<ComponentType::Render>(.5f);

        //const auto &component = entity.getComponent<ComponentType::Transform>();
         */

        initLoop();
    }

    /**
     * Fixed update
     */
    void Engine::fixedUpdate() {

    }

    /**
     * Free update
     */
    void Engine::update(double alpha) {
        m_context->clear();

        // For alpha I need to create transform old and new...
        //m_ecs->updateSystem<Render::RenderSystem>();

        m_context->swapBuffers();
    }

    /**
     * Fixed timestamp loop
     */
    void Engine::initLoop() {

        /**
         * LOOP
         */

        double t = 0.0;
        double dt = 0.01;

        double currentTime = m_context->getTime();
        double accumulator = 0.0;

        bool test = m_context->isRunning();
        std::cout << test << std::endl;

        while(m_context->isRunning())
        {
            double newTime = m_context->getTime();
            double frameTime = newTime - currentTime;
            if( frameTime > 0.25 )
                frameTime = 0.25;

            currentTime = newTime;
            accumulator += frameTime;

            while( accumulator >= dt )
            {
                // logic here
                m_context->processInput();
                fixedUpdate();

                t += dt;
                accumulator -= dt;
            }

            const double alpha = accumulator / dt;

            // render here
            m_context->pollEvents();
            update(alpha);
        }

    }

}