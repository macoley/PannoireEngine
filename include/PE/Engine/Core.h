#ifndef PANNOIREENGINE_CORE_H
#define PANNOIREENGINE_CORE_H

#include <memory>
#include <iostream>

#include "PE/ECS/ECS.h"
#include "PE/Resource/ResourceManager.h"
#include "PE/Render/Render.h"
#include "PE/Utils/Utils.h"

#include "PE/Render/Texture.h"
#include "PE/Render/Shader.h"

#include "Properties.h"
#include "Scene.h"

namespace PE::Engine {

    class Core {
    public:
        Core();
        virtual ~Core();
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


#endif //PANNOIREENGINE_CORE_H
