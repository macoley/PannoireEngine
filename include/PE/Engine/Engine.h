#ifndef PANNOIREENGINE_ENGINE_H
#define PANNOIREENGINE_ENGINE_H

#include <memory>
#include <iostream>

#include "PE/Engine/ECS.h"
#include "PE/Engine/ComponentType.h"
#include "PE/Engine/System.h"

#include "PE/Render/RenderSystem.h"

namespace PE::Engine {

    using namespace PE;

    class Engine {
    public:
        Engine();

        void init();

    private:
        std::shared_ptr<ECS> m_ecs;
    };

}


#endif //PANNOIREENGINE_ENGINE_H
