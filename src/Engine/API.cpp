
#include <PE/Engine/API.h>

namespace PE::Engine {

    using namespace PE::Engine::Component;

    API::API(API::sePtr t_engine, API::ecsPtr t_ecs)
        : m_engine(std::move(t_engine)), m_ecs(std::move(t_ecs))
    {
        // Transform
        reg_component(Transform)
        reg_component_val(Transform, xPos)
        reg_component_val(Transform, yPos)
        reg_component_val(Transform, zPos)

        reg_component_val(Transform, xScale)
        reg_component_val(Transform, yScale)
        reg_component_val(Transform, zScale)

        reg_component_val(Transform, xAngle)
        reg_component_val(Transform, yAngle)
        reg_component_val(Transform, zAngle)


        m_engine->registerMethod(&API::getComponent, "getComponent");
        m_engine->registerGlobal(this, "API");
    }


    std::shared_ptr<ComponentBind<Transform>> API::getComponent(uint32_t entity, uint32_t component) {
        return std::make_shared<ComponentBind<Transform>>(m_ecs->getComponentUnsafe<Transform>(entity));
    }



}