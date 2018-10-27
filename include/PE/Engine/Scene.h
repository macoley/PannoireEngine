#ifndef PANNOIREENGINE_SCENE_H
#define PANNOIREENGINE_SCENE_H

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "PE/ECS/ECS.h"
#include "PE/Utils/Utils.h"
#include "Properties.h"
#include "Component.h"

namespace PE::Engine {

    class Scene {
    public:
        explicit Scene(const std::string & path, std::shared_ptr<ECS::ECS>);
        virtual ~Scene();

    private:
        void makeEntity(YAML::Node);

        std::vector<ECS::Entity> m_entities;
        std::shared_ptr<ECS::ECS> m_ecs;
        Properites m_properies;
    };


}

#endif //PANNOIREENGINE_SCENE_H
