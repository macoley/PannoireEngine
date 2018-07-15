#include "PE/Engine/Engine.h"


namespace PE::Engine {


    Engine::Engine()
            : m_ecs(std::make_shared<ECS>()),
              m_context(std::make_shared<PE::Render::Context>())
    {
    }

    void Engine::init() {
        m_ecs->registerComponent<ComponentType::Transform>();
        m_ecs->registerComponent<ComponentType::Render>();

        m_ecs->createSystem<Render::RenderSystem>(m_context);

        auto entity = m_ecs->createEntity();
        auto entity2 = m_ecs->createEntity();
        entity.assignComponent<ComponentType::Transform>(.5f, .2f, .1f);
        entity.assignComponent<ComponentType::Render>(.5f);
        entity2.assignComponent<ComponentType::Transform>(.5f, .2f, .1f);
        entity2.assignComponent<ComponentType::Render>(.5f);

        //const auto &component = entity.getComponent<ComponentType::Transform>();

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
        // For alpha I need to create transform old and new...
        m_ecs->updateSystem<Render::RenderSystem>();
    }

    /**
     * Fixed timestamp loop
     */
    void Engine::initLoop() {
        double t = 0.0;
        double dt = 0.01;

        double currentTime = m_context->getTime();
        double accumulator = 0.0;

        while(m_context->isRunning())
        {
            /*
             * LOGIC
             */

            //
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
            m_context->update();
            update(alpha);
        }
    }


}