#ifndef PANNOIREENGINE_SCENE_H
#define PANNOIREENGINE_SCENE_H

#include <string>

#include "PE/ECS/ECS.h"
#include "Properties.h"

namespace PE::Engine {

    class Scene {
    public:
        explicit Scene(const std::string & path);
        virtual ~Scene();

        void applyToECS(std::shared_ptr<ECS::ECS> ecs);

    private:
        Properites m_properies;
    };


}

#endif //PANNOIREENGINE_SCENE_H
