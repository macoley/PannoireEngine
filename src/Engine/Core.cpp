#include "PE/Engine/Core.h"


namespace PE::Engine {

    Core::Core()
            : m_ecs(ECS::MakeECS()),
              m_res_manager(Resource::MakeManager()) {}

    void Core::init() {
        // UTILS
        Utils::Locator::provide(new Utils::ConsoleLogger());
        Utils::log("Core initializing");

        // Resources
        m_res_manager->registerResource<Resource::Properties>();
        m_res_manager->registerResource<Render::Mesh>();
        m_res_manager->registerResource<Render::Texture>();
        m_res_manager->registerResource<Render::Material>();
        m_res_manager->registerResource<Render::Model>(m_res_manager);
        m_res_manager->registerResource<Render::Shader>(m_res_manager);
        m_res_manager->registerResource<Render::VertexShader>();
        m_res_manager->registerResource<Render::FragmentShader>();
        m_res_manager->registerResource<Engine::Script>();
        m_res_manager->registerResource<Engine::Scene>(m_res_manager, m_ecs);


        // APP CONFIG

        auto config = m_res_manager->load<Resource::Properties>("config.yml");

        // RENDER CONTEXT
        m_context = Render::createContext(
                config->get<std::string>("title"),
                config->get<uint32_t>("width"),
                config->get<uint32_t>("height")
        );
        Render::Texture::placeholdersInit();

        // MAIN SCENE
        m_camera = std::make_shared<Render::Camera>(
                config->get<uint32_t>("width"),
                config->get<uint32_t>("height")
        );


        m_context->setInputCallback([&](uint32_t key, uint32_t action) {

            if (key == 68) // d
                m_camera->move(Render::Camera_Movement::RIGHT);

            if (key == 65) // a
                m_camera->move(Render::Camera_Movement::LEFT);

            if (key == 87) // w
                m_camera->move(Render::Camera_Movement::FORWARD);

            if (key == 83) // s
                m_camera->move(Render::Camera_Movement::BACKWARD);
        });


        m_shader = m_res_manager->load<Render::Shader>("shader.yml");
        m_scene = m_res_manager->load<Engine::Scene>(config->get<std::string>("main_scene"));


        // RENDERER
        m_renderer = std::make_shared<Render::Renderer>(m_camera);

        m_ecs->view<Component::Transform, Component::Camera>()
                .each([&](const ECS::Entity entity, Component::Transform &t, Component::Camera &c) {
                    m_camera->setPos(t.xPos, t.yPos, t.zPos);
                    m_camera->rotate(t.xAngle, t.yAngle);
                    m_camera->zoom(c.zoom);
                });


        auto monitor = std::make_unique<Resource::WindowsFileMonitor>();

        monitor->watchDirectory("res", [&](const std::string &filename) {
            Utils::log(filename + " has been changed!");
            m_res_manager->addMsg<Resource::ResourceEvents::FILE_CHANGED>(Resource::FileInfo{filename});
        });


        // SCRIPTING
        //m_script = m_res_manager->load<Engine::Script>("res/script.js");
        m_script = m_res_manager->load<Engine::Script>("res/Rotate.js");

        using API = Scripting<ECS::Manager>;
        auto m_scripting = API::getInstance();
        m_scripting->setECS(m_ecs);

        ctx = duk_create_heap_default();


        dukglue_register_constructor<API>(ctx, "API");
        dukglue_register_function(ctx, API::getInstance, "getAPI");
        dukglue_register_method(ctx, &API::rotateAll, "rotateAll");

        initLoop();
    }

    /**
     * Fixed update
     */
    void Core::fixedUpdate() {
        m_context->processInput();

        m_renderer->reset();

        m_res_manager->dispatch();

        m_script->updateFixed();

        m_ecs->view<Component::Transform, Component::Model>()
                .each([&](const ECS::Entity entity, Component::Transform &t, Component::Model &r) {

                    for (auto &element : m_res_manager->get<Render::Model>(r.resIndex)->getObject()) {
                        m_renderer->add(
                                element.first.get(), element.second.get(),
                                t.xPos, t.yPos, t.zPos,
                                t.xAngle, t.yAngle, t.zAngle,
                                t.xScale, t.yScale, t.zScale
                        );
                    }

                });

        m_renderer->sort();
    }

    /**
     * Free update
     */
    void Core::update(double alpha) {
        m_context->pollEvents();

        m_context->render([&]() {
            m_renderer->render(*m_shader, alpha);
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
        duk_destroy_heap(ctx);
        Render::Texture::placeholdersDestroy();

        Utils::log("Engine turned off");
    }

}