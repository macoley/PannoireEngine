#ifndef PANNOIREENGINE_ENTITY_H
#define PANNOIREENGINE_ENTITY_H

#include <cstdint>

#include "PE/Engine/Common.h"

namespace PE::Engine {

    class ECS;

    /**
     * EntityWrapper for ID
     */
    class Entity {
        friend class ECS;

    public:
        /**
         * EntityID
         */
        struct ID {
            ID() : m_id(0) {}
            explicit ID(uint64_t t_id) : m_id(t_id) {}
            explicit ID(uint32_t t_index, uint32_t t_version)
                    : m_id(static_cast<uint64_t>(t_index) |
                           static_cast<uint64_t>(t_version) << 32UL) {}

            bool operator==(const ID &t_other) const { return m_id == t_other.m_id; }
            bool operator!=(const ID &t_other) const { return m_id != t_other.m_id; }
            bool operator<(const ID &t_other) const { return m_id < t_other.m_id; }

            inline uint32_t index() const { return static_cast<uint32_t>(m_id & 0xffffffffUL); }
            inline uint32_t version() const { return static_cast<uint32_t>(m_id >> 32UL); }

        private:
            uint64_t m_id;
        };

        // Wrappers (implemented in ECS.h)
        template <typename C, typename ... Args>
        void assignComponent(Args && ... args);

        template<typename C>
        void removeComponent();

        template <typename C>
        C& getComponent() const;

        // Get ID
        inline const ID getID() {
            return m_id;
        }

    private:
        Entity(std::shared_ptr<ECS> t_manager_ptr, ID t_id)
                : m_manager_ptr(std::move(t_manager_ptr)), m_id(t_id) {}

        std::shared_ptr<ECS> m_manager_ptr;
        ID m_id;
    };

}


#endif //PANNOIREENGINE_ENTITY_H
