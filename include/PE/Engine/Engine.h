#ifndef PANNOIREENGINE_ENGINE_H
#define PANNOIREENGINE_ENGINE_H

#include <memory>
#include <iostream>

#include "PE/Engine/ECS.h"
#include "PE/Engine/ComponentType.h"
#include "PE/Engine/System.h"

namespace PE::Engine {

    class RenderSystem : public System<ComponentType::Transform, ComponentType::Render>
    {
    public:
        virtual void update(ComponentType::Transform * t, ComponentType::Render * r) {
            std::cout << "UPDATE \n";
            std::cout << t[0].x;
            std::cout << t[0].y;
            std::cout << t[0].z;
        }

        void test(int a) final {
            std::cout << "UPDATE TEST \n";
        }
    };

    class Engine {
    public:
        Engine();

        void init();

    private:
        std::shared_ptr<ECS> m_ecs;
    };

}


#endif //PANNOIREENGINE_ENGINE_H
