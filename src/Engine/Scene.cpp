
#include <PE/Engine/Scene.h>

#include "PE/Engine/Scene.h"

namespace PE::Engine {

    void Scene::load(const std::string &path) {
        auto properties = m_manager->load<Resource::Properties>(path);
        auto entitiesNode = properties->get<YAML::Node>("entities");
        auto backgroundColorNode = properties->get<YAML::Node>("backgroundColor");

        m_bg = {backgroundColorNode[0].as<float>(), backgroundColorNode[1].as<float>(), backgroundColorNode[2].as<float>()};

        for(const YAML::Node &n : entitiesNode) {
            makeEntity(n);
        }
    }

    Scene::~Scene() {
        for(auto &[id, handle] : m_scripts)
            handle->deleteInstance(id);

        for(auto &entity : m_entities)
            m_ecs->destroyEntity(entity);

        m_entities.clear();
    }

    /**
     * Helper
     */
    void Scene::makeEntity(const YAML::Node &entityNode) {
        auto entity = m_ecs->createEntity();
        auto componentsNode = entityNode["components"];

        for (std::size_t i = 0; i < componentsNode.size(); i++) {
            auto component = componentsNode[i];
            auto type = component["type"].as<std::string>();

            // Transform component
            if(type == "Transform")
            {
                m_ecs->assignComponent<Component::Transform>(
                        entity,
                        component["xPos"].as<float>(),
                        component["yPos"].as<float>(),
                        component["zPos"].as<float>(),
                        component["xScale"].as<float>(),
                        component["yScale"].as<float>(),
                        component["zScale"].as<float>(),
                        component["xAngle"].as<float>(),
                        component["yAngle"].as<float>(),
                        component["zAngle"].as<float>()
                );

                continue;
            }

            if(type == "Model")
            {
                auto res = m_manager->load<Render::Model>(component["path"].as<std::string>());
                m_models.push_back(res);

                m_ecs->assignComponent<Component::Model>(
                        entity,
                        static_cast<uint32_t>(res.getIndex())
                );

                continue;
            }

            if(type == "Camera")
            {
                m_ecs->assignComponent<Component::Camera>(
                        entity,
                        component["zoom"].as<float>()
                );

                continue;
            }

            if(type == "Light")
            {
                m_ecs->assignComponent<Component::Light>(
                        entity,
                        component["color"][0].as<float>(),
                        component["color"][1].as<float>(),
                        component["color"][2].as<float>()
                );

                continue;
            }

            if(type == "Script")
            {
                auto res = m_manager->load<Scripting::Script>(component["path"].as<std::string>());
                auto id = res->instantiate(ECS::getIndex(entity));

                m_scripts.push_back({id, res});

                m_ecs->assignComponent<Component::Script>(
                        entity,
                        static_cast<uint32_t>(res.getIndex())
                );

                continue;
            }

            // Todo other components
        }

        m_entities.push_back(entity);
    }

}