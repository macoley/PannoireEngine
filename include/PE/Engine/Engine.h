#ifndef PANNOIREENGINE_ENGINE_H
#define PANNOIREENGINE_ENGINE_H

#include <memory>

#include "PE/Engine/ECS.h"
#include "PE/Engine/ComponentType.h"
#include "PE/Render/Render.h"
#include "PE/Engine/Pool.h"

namespace PE::Engine {

    class Engine {
    public:
        Engine();

        void init();

    private:
        std::shared_ptr<ECS> m_ecs;
    };

}


#endif //PANNOIREENGINE_ENGINE_H
