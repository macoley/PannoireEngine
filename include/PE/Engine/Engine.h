#ifndef PANNOIREENGINE_ENGINE_H
#define PANNOIREENGINE_ENGINE_H

#include "PE/Render/Render.h"
#include "PE/Engine/Pool.h"

namespace PE::Engine {

    class Engine {
    public:
        Engine();

        template <typename C, typename ... Types>
        void assignComponent(Types ... args);

    private:
        std::vector<MemoryPool*> m_components_pools;
    };

    /*
     * ECS
     */


    class ECS {
    public:
        struct ID {
            ID() : m_id(0) {}
            explicit ID(uint64_t t_id) : m_id(t_id) {}
            explicit ID(uint32_t t_index, uint32_t t_version)
                    : m_id( static_cast<uint64_t>(t_index) |
                            static_cast<uint64_t>(t_version) << 32UL) {}

            bool operator == (const ID &t_other) const { return m_id == t_other.m_id; }
            bool operator != (const ID &t_other) const { return m_id != t_other.m_id; }
            bool operator <  (const ID &t_other) const { return m_id < t_other.m_id; }

            uint32_t index() const { return static_cast<uint32_t>(m_id & 0xffffffffUL); }
            uint32_t version() const { return static_cast<uint32_t>(m_id >> 32UL); }

        private:
            uint64_t m_id;
        };

    };

    /*
     * ENTITY
     */
    class EntityHandler {
    public:
        // Get components (tuple)
        // Add component


    private:
        friend class ECS;

        std::shared_ptr<ECS> m_manager_ptr;
        ECS::ID m_id;
    };


    /*
     * COMPONENT
     */
    template <typename T>
    class ComponentHandler {
    public:

        // Todo get Entity
        // Todo subscribe other <Component> in other* Entity(Handler)

    private:
        friend class ECS;

        std::shared_ptr<ECS> m_manager_ptr;
        ECS::ID m_id;
    };
}


#endif //PANNOIREENGINE_ENGINE_H
