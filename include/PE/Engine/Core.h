#ifndef PANNOIREENGINE_CORE_H
#define PANNOIREENGINE_CORE_H

#include <memory>
#include <iostream>
#include <cstring>

#include "PE/ECS/ECS.h"
#include "PE/Resource/Resource.h"
#include "PE/Render/Render.h"
#include "PE/Utils/Utils.h"

#include "PE/Render/Texture.h"
#include "PE/Render/Shader.h"
#include "PE/Render/Model.h"
#include "PE/Render/Renderer.h"

#include "PE/Resource/Properties.h"
#include "PE/Resource/WindowsFileMonitor.h"

#include "Scene.h"
#include "LoggerDecorator.h"
#include "Component.h"

#include "PE/Render/Renderer.h"
#include "PE/Scripting/Scripting.h"

#include "API.h"


namespace PE::Engine {

    class Core {
    public:
        Core();
        virtual ~Core();
        void init();

    private:
        void fixedUpdate();
        void update(double alpha);
        void initLoop();

        std::shared_ptr<ECS::Manager> m_ecs;
        std::shared_ptr<Resource::ResourceManager> m_res_manager;
        std::shared_ptr<Render::Context> m_context;
        std::shared_ptr<Render::Camera> m_camera;
        std::shared_ptr<Render::Renderer> m_renderer;
        std::shared_ptr<Scripting::ScriptEngine> m_scripting;
        std::shared_ptr<Engine::API> m_api;

        //std::shared_ptr<Engine::Scripting<ECS::Manager>> m_scripting;

        Resource::ResourceHandle<Engine::Scene> m_scene;
        Resource::ResourceHandle<Render::Shader> m_shader;
    };

}


#endif //PANNOIREENGINE_CORE_H
