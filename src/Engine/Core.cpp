#include "PE/Engine/Core.h"

namespace PE::Engine {

    Core::Core()
            : m_ecs(new ECS::ECS),
              m_res_manager(new Resource::ResourceManager)
    {}

    void Core::init() {
        // UTILS
        Utils::Locator::provide(new Utils::ConsoleLogger());
        Utils::log("Core initializing");

        // RENDERER SYSTEM
        Render::init();
        m_context = std::shared_ptr<Render::Context>(Render::createContext());

        // RESOURCE MANAGER
        auto texture = m_res_manager->load<Render::Texture>("container.jpg");
        auto shader = m_res_manager->load<Render::Shader>("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

        auto config = m_res_manager->load<Engine::Properites>();
        config->set("width", 450);
        config->set("height", 150);
        config->save("config.yml");

        auto scene = m_res_manager->load<Engine::Scene>("scene.yml");

        //m_ecs->registerComponent<Engine::Transform>();
        //m_ecs->registerComponent<Engine::Render>();

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
    void Core::fixedUpdate() {

    }

    /**
     * Free update
     */
    void Core::update(double alpha) {
        m_context->clear();

        // For alpha I need to create transform old and new...
        //m_ecs->updateSystem<Render::RenderSystem>();

        m_context->swapBuffers();
    }

    /**
     * Fixed timestamp loop
     */
    void Core::initLoop() {
        Utils::log("Main loop initializing");

        /**
         * LOOP
         */

        double t = 0.0;
        double dt = 0.01;

        double currentTime = m_context->getTime();
        double accumulator = 0.0;

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

    Core::~Core() {
        Utils::log("Engine turned off");
    }

}