#ifndef PANNOIREENGINE_ENGINE_H
#define PANNOIREENGINE_ENGINE_H

#include <memory>
#include <iostream>

#include "PE/ECS/ECS.h"
#include "PE/Resource/Exporter.h"

namespace PE {

    class Engine {
    public:
        Engine();
        void init();

    private:
        void fixedUpdate();
        void update(double alpha);
        void initLoop();

        std::shared_ptr<ECS::ECS> m_ecs;
    };

}


#endif //PANNOIREENGINE_ENGINE_H
