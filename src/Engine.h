#ifndef PANNOIREENGINE_ENGINE_H
#define PANNOIREENGINE_ENGINE_H

#include <memory>
#include <iostream>

#include "PE/ECS/ECS.h"
#include "PE/Resource/ResourceManager.h"
#include "PE/Render/Texture.h"
#include "PE/Render/Shader.h"
#include "PE/Component/Types.h"
#include "PE/Render/Context.h"

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
        std::shared_ptr<Resource::ResourceManager> m_res_manager;
        std::shared_ptr<Render::Context> m_context;
    };

}


#endif //PANNOIREENGINE_ENGINE_H
