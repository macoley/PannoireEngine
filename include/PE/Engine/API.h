#ifndef PANNOIREENGINE_API_H
#define PANNOIREENGINE_API_H

#include "API_Helper.h"
#include "PE/Scripting/ScriptEngine.h"
#include "PE/ECS/ECS.h"
#include "Component.h"
#include <any>

namespace PE::Engine {

    // Forward
    template <typename T> struct ComponentBind;

    // Transform
    comp_wrapper_begin(Component::Transform)
        getter_setter(float, xPos);
        getter_setter(float, yPos);
        getter_setter(float, zPos);

        getter_setter(float, xScale);
        getter_setter(float, yScale);
        getter_setter(float, zScale);

        getter_setter(float, xAngle);
        getter_setter(float, yAngle);
        getter_setter(float, zAngle);
    comp_wrapper_end()

    /**
     * API
     */
    class API {
        using sePtr = std::shared_ptr<Scripting::ScriptEngine>;
        using ecsPtr = std::shared_ptr<ECS::Manager>;

    public:
        explicit API(sePtr, ecsPtr);

        std::shared_ptr<ComponentBind<Component::Transform>> getComponent(uint32_t entity, uint32_t component);

    private:
        sePtr m_engine;
        ecsPtr m_ecs;
    };


}

#endif //PANNOIREENGINE_API_H
