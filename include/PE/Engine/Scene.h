#ifndef PANNOIREENGINE_SCENE_H
#define PANNOIREENGINE_SCENE_H

#include <string>
#include <vector>
#include <memory>

#include <yaml-cpp/yaml.h>

#include "PE/ECS/ECS.h"
#include "PE/Utils/Utils.h"
#include "PE/Resource/Resource.h"
#include "PE/Resource/Properties.h"
#include "PE/Render/Shader.h"
#include "PE/Render/Model.h"
#include "PE/Render/Camera.h"

#include "Component.h"

namespace PE::Engine {

    class Scene : public Resource::IResource {
        using PropertiesHandle = Resource::ResourceHandle<Resource::Properties>;
        using ShaderHandle = Resource::ResourceHandle<Render::Shader>;

        using ResManagerPtr = std::shared_ptr<Resource::ResourceManager>;
        using ECSPtr = std::shared_ptr<ECS::Manager>;

    public:
        explicit Scene(ResManagerPtr manager, ECSPtr ecs)
                : m_ecs(std::move(ecs)),
                  m_manager(std::move(manager))
        {
            m_shader = m_manager->load<Render::Shader>("shader.yml", m_manager);
            m_model = m_manager->load<Render::Model>("res/ForestScene.obj", m_manager);

            auto config = m_manager->load<Resource::Properties>("config.yml");

            m_camera.reset(new Render::Camera(config->get<uint32_t>("width"), config->get<uint32_t>("height"), 0.5f, 0.5f, 15.0f));
        }

        Scene() = delete;
        virtual ~Scene();

        void load(const std::string& path) override;
        void draw();

    private:
        void makeEntity(YAML::Node);

        std::vector<ECS::Entity> m_entities;
        ECSPtr m_ecs;
        ResManagerPtr m_manager;

        ShaderHandle m_shader;
        Resource::ResourceHandle<Render::Model> m_model;
        std::unique_ptr<Render::Camera> m_camera;
    };


}

#endif //PANNOIREENGINE_SCENE_H
