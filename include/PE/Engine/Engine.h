#ifndef PANNOIREENGINE_ENGINE_H
#define PANNOIREENGINE_ENGINE_H

#include <memory>
#include <iostream>

#include "PE/Engine/ECS.h"
#include "PE/Engine/ComponentType.h"
#include "PE/Engine/System.h"
#include "PE/Engine/IContext.h"

#include "PE/Render/RenderSystem.h"
#include "PE/Render/Context.h"

namespace PE::Engine {

    using namespace PE;

    class Engine {
    public:
        Engine();

        void init();

    private:
        inline void fixedUpdate();
        inline void update(double alpha);
        inline void initLoop();

        std::shared_ptr<ECS> m_ecs;
        std::shared_ptr<IContext> m_context;
    };

}


#endif //PANNOIREENGINE_ENGINE_H
