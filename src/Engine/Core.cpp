#include "PE/Engine/Core.h"


namespace PE::Engine {

    Core::Core()
            : m_ecs(ECS::MakeECS()),
              m_res_manager(Resource::MakeManager()) {}

    void Core::init() {
        // UTILS
        Utils::Locator::provide(new Utils::ConsoleLogger());
        Utils::log("Core initializing");

        // APP CONFIG
        auto config = m_res_manager->load<Resource::Properties>("config.yml");

        // RENDERER SYSTEM
        Render::init();
        m_context = Render::createContext(
                config->get<std::string>("title"),
                config->get<uint32_t>("width"),
                config->get<uint32_t>("height")
        );

        m_context->setResizeCallback([&](uint32_t width, uint32_t height) {
           config->set("width", width);
           config->set("height", height);
        });

        // MAIN SCENE
        scene = m_res_manager->load<Engine::Scene>(config->get<std::string>("main_scene"), m_res_manager, m_ecs);

        initLoop();
    }

    /**
     * Fixed update
     */
    void Core::fixedUpdate() {
        m_context->processInput();
    }

    /**
     * Free update
     */
    void Core::update(double alpha) {
        m_context->pollEvents();

        m_context->render([&]() {
            scene->draw();
        });
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

        while (m_context->isRunning()) {
            double newTime = m_context->getTime();
            double frameTime = newTime - currentTime;

            if (frameTime > 0.25)
                frameTime = 0.25;

            currentTime = newTime;
            accumulator += frameTime;

            while (accumulator >= dt) {
                // logic here
                fixedUpdate();

                t += dt;
                accumulator -= dt;
            }

            const double alpha = accumulator / dt;

            // render here
            update(alpha);
        }

    }

    Core::~Core() {
        Utils::log("Engine turned off");
    }

}