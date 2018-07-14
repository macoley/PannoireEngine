#ifndef PANNOIREENGINE_SYSTEM_H
#define PANNOIREENGINE_SYSTEM_H

#include <vector>
#include <cstdint>
#include <algorithm>
#include <tuple>
#include <functional>

#include "PE/Engine/Common.h"
#include "PE/Engine/Utils.h"

namespace PE::Engine {

    class ECS;

    /**
     * Base system
     */
    class BaseSystem {
        friend class ECS;

    protected:
        BaseSystem(ComponentMask t_mask) : m_component_mask(t_mask) {}

        inline const std::vector<uint32_t> & getEntities() {
            return m_entities;
        }

        static SystemIndex m_system_counter;

    private:
        ComponentMask m_component_mask;
        std::vector<uint32_t> m_entities;

        inline void addEntity(uint32_t index) {
            m_entities.push_back(index);
            std::sort(m_entities.begin(), m_entities.end());
        }

        inline void removeEntity(uint32_t index) {
            m_entities.push_back(index);
            std::sort(m_entities.begin(), m_entities.end());
        }
    };

    /**
     * Interface for systems
     * @tparam Cargs
     */
    template <typename ... Components>
    class System : public BaseSystem {
        friend class ECS;

    public:
        virtual void update(Components * ...) = 0;

        virtual void test(int a) {};

    protected:
        System() : BaseSystem(getMask<Components ...>()) {}

    private:
        static SystemIndex getSystemIndex();

        inline System<Components...> *getSystem() {
            return reinterpret_cast<System<Components...> *>(this);
        }
    };


    /*
     * System index getter
     */
    template <typename ... Components>
    SystemIndex System<Components ...>::getSystemIndex() {
        static SystemIndex system_index = m_system_counter++;
        return system_index;
    }

}

#endif //PANNOIREENGINE_SYSTEM_H
