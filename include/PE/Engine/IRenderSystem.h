#ifndef PANNOIREENGINE_IRENDERSYSTEM_H
#define PANNOIREENGINE_IRENDERSYSTEM_H

#include "PE/Engine/ComponentType.h"

namespace PE::Engine {

    using namespace PE::Engine::ComponentType;

    class IRenderSystem {
    public:
        virtual ~IRenderSystem() = default;

        virtual void render(Transform *t_transform, Render *t_render, std::size_t t_amount) = 0;
    };


}


#endif //PANNOIREENGINE_IRENDERSYSTEM_H
