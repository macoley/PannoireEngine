#ifndef PANNOIREENGINE_SCENE_H
#define PANNOIREENGINE_SCENE_H

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "PE/ECS/ECS.h"
#include "PE/Utils/Utils.h"
#include "PE/Resource/Resource.h"
#include "PE/Resource/Properties.h"

#include "Component.h"

namespace PE::Engine {

    class Scene : public Resource::IResource {
        using PropertiesHandle = Resource::ResourceHandle<Resource::Properties>;

        using ResManagerPtr = std::shared_ptr<Resource::ResourceManager>;
        using ECSPtr = std::shared_ptr<ECS::Manager>;

    public:
        explicit Scene(ResManagerPtr manager, ECSPtr ecs)
                : m_ecs(std::move(ecs)),
                  m_manager(std::move(manager))
        {}

        Scene() = delete;
        virtual ~Scene();

        void load(const std::string& path) override;

    private:
        void makeEntity(YAML::Node);

        std::vector<ECS::Entity> m_entities;
        ECSPtr m_ecs;
        ResManagerPtr m_manager;

        PropertiesHandle m_properties;
    };


}

#endif //PANNOIREENGINE_SCENE_H
