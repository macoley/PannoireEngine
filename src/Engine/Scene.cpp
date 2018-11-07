
#include "PE/Engine/Scene.h"

namespace PE::Engine {

    /**
     * Scene Constructor
     * @param path
     * @param ecs
     */
    Scene::Scene(const std::string &path, std::shared_ptr<ECS::Manager> ecs)
        : m_properies(path),
          m_ecs(std::move(ecs))
    {
        auto entitiesNode = m_properies.get<YAML::Node>("entities");

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
                        component["x"].as<double>(),
                        component["y"].as<double>(),
                        component["z"].as<double>()
                );
            }

            // Todo other components
        }

        m_entities.push_back(entity);
    }

}