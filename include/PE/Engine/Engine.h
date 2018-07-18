#ifndef PANNOIREENGINE_ENGINE_H
#define PANNOIREENGINE_ENGINE_H

#include <memory>
#include <iostream>

#include "PE/Engine/ECS.h"
#include "PE/Engine/ComponentType.h"
#include "PE/Engine/System.h"
#include "PE/Engine/IContext.h"

namespace PE::Engine {

    using namespace PE;

    class Engine {
    public:
        Engine(std::shared_ptr<IContext> t_context);

        void init();

    private:
        void fixedUpdate();
        void update(double alpha);
        void initLoop();

        std::shared_ptr<ECS> m_ecs;
        std::shared_ptr<IContext> m_context;
    };

}


#endif //PANNOIREENGINE_ENGINE_H
