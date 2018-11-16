
#include <PE/Engine/Scene.h>

#include "PE/Engine/Scene.h"

namespace PE::Engine {

    void Scene::load(const std::string &path) {
        auto properties = m_manager->load<Resource::Properties>(path);
        auto entitiesNode = properties->get<YAML::Node>("entities");

        for (std::size_t i = 0; i < entitiesNode.size(); i++) {
            makeEntity(entitiesNode[i]);
        }
    }

    Scene::~Scene() {
        for(auto &entity : m_entities)
            m_ecs->destroyEntity(entity);

        m_entities.clear();
    }

    /**
     * Helper
     */
    void Scene::makeEntity(YAML::Node entityNode) {
        auto entity = m_ecs->createEntity();
        auto componentsNode = entityNode["components"];

        for (std::size_t i = 0; i < componentsNode.size(); i++) {
            auto component = componentsNode[i];

            // Transform component
            if(component["type"].as<std::string>() == "Transform")
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

            // Todo other components
        }

        m_entities.push_back(entity);
    }

    /**
     * Draw
     */
    void Scene::draw() {
        m_shader->use();

        m_shader->set("projection", m_camera->getProjection());
        m_shader->set("view", m_camera->getView());

        m_ecs->view<Component::Transform>()
                .each([&](const ECS::Entity entity, Component::Transform& t) {
                    m_model->draw(*m_shader, t.xPos, t.yPos, t.zPos, t.xScale, t.yScale, t.zScale, t.xAngle, t.yAngle, t.zAngle);
                });
    }


}