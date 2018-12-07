#ifndef PE_ENGINE_SCRIPTING_H
#define PE_ENGINE_SCRIPTING_H

#include <memory>

#include "PE/ECS/ECS.h"
#include "Component.h"

namespace PE::Engine {

    template<typename TECS>
    class Scripting {
        using ClassPtr = std::unique_ptr<Scripting>;
        using ECSPtr = std::shared_ptr<TECS>;

    public:
        static Scripting* getInstance() {
            if(!m_instance)
                m_instance = std::make_unique<Scripting>();

            return m_instance.get();
        }

        void rotateAll(float speed) {
            m_ecs->template view<Component::Transform>()
                    .each([&](const ECS::Entity entity, Component::Transform &t) {
                        t.yAngle += speed;
                    });
        };

        void setECS(ECSPtr ecs);

        Scripting() = default;
        virtual ~Scripting() = default;
    private:
        ECSPtr m_ecs;
        static ClassPtr m_instance;
    };

    template<typename TECS>
    typename Scripting<TECS>::ClassPtr Scripting<TECS>::m_instance = nullptr;

    /**
     * ECS dependency injector
     * @tparam TECS
     * @param ecs
     */
    template<typename TECS>
    void Scripting<TECS>::setECS(Scripting::ECSPtr ecs) {
        m_ecs = std::move(ecs);
    }
}


#endif //PE_ENGINE_SCRIPTING_H
