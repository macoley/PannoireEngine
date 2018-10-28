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

        // APP CONFIG
        auto config = m_res_manager->load<LoggerDecorator<Engine::Properites>>("Application properties", "config.yml");

        // RENDERER SYSTEM
        Render::init();
        m_context = std::shared_ptr<Render::Context>(
                Render::createContext(
                    config->get<std::string>("title"),
                    config->get<uint32_t>("width"),
                    config->get<uint32_t>("height")
                )
        );

        // RESOURCE MANAGER
        auto texture = m_res_manager->load<LoggerDecorator<Render::Texture>>("Container texture", "container.jpg");
        auto shader = m_res_manager->load<LoggerDecorator<Render::Shader>>("Shader basic", "shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

        m_ecs->registerComponent<Component::Transform>();

        // MAIN SCENE
        auto scene = m_res_manager->load<LoggerDecorator<Engine::Scene>>("Main scene", config->get<std::string>("main_scene"), m_ecs);

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