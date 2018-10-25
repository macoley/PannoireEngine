
#include "PE/Engine/Scene.h"

namespace PE::Engine {

    Scene::Scene(const std::string &path)
        : m_properies(path)
    {

    }

    Scene::~Scene() {

    }

    /**
     * TODO Make some factory?
     * @param ecs
     */
    void Scene::applyToECS(std::shared_ptr<ECS::ECS> ecs) {
        /*
        auto entity = m_ecs->createEntity();
        auto entity2 = m_ecs->createEntity();
        entity.assignComponent<ComponentType::Transform>(.5f, .2f, .1f);
        entity.assignComponent<ComponentType::Render>(.5f);
        entity2.assignComponent<ComponentType::Transform>(.5f, .2f, .1f);
        entity2.assignComponent<ComponentType::Render>(.5f);
         */
    }
}