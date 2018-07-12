#ifndef PANNOIREENGINE_ECS_H
#define PANNOIREENGINE_ECS_H

#include <memory>
#include <vector>
#include <bitset>

#include "PE/Engine/Pool.h"
#include "PE/Engine/Entity.h"
#include "PE/Engine/Component.h"
#include "PE/Engine/ISystem.h"

namespace PE::Engine {

    class Entity;
    class BaseComponent;
    class ISystem;

    template<typename C>
    class Component;

    /**
     * ECS Architecture Manager
     * Please create with shared_ptr
     */
    class ECS : public std::enable_shared_from_this<ECS> {
    public:
        /**
         * Max components
         */
        const static std::size_t MAX_COMPONENTS = 64;

        /**
         * Component Mask
         */
        typedef std::bitset<MAX_COMPONENTS> ComponentMask;

        /*
         * Entities
         */
        Entity createEntity();

        void destroyEntity(Entity::ID t_id);

        Entity getEntity(Entity::ID t_id);

        /*
         * Components
         */

        template<typename C>
        void registerComponent();

        template <typename C, typename ... Args>
        void assignComponent(Entity::ID t_id, Args && ... args);

        template<typename C>
        void removeComponent(Entity::ID t_id);

        template <typename C>
        C& getComponent(Entity::ID t_id) const;


    private:
        typedef std::size_t FamilyIndex;

        /*
         * Entities Private functions
         */
        void accomodateEntity(uint32_t t_index);

        template <typename C>
        ObjectPool<C>* getComponentPool() const;

        void removeComponent(Entity::ID t_id, FamilyIndex t_family);

        template <typename C>
        inline FamilyIndex getComponentFamily() const {
            return Component<typename std::remove_const<C>::type>::getFamily();
        }

        /**
         * Main ECS Data
         */
        uint32_t m_entity_index_counter = 0;                                // Index counter
        std::vector<uint32_t> m_free_entity_list;                           // List of free indexes
        std::vector<uint32_t> m_entity_version;                             // Versions of entity

        std::vector<ComponentMask> m_entity_component_mask;                 // Mask of components for each entity
        std::vector<MemoryPool*> m_component_pools;         // Memory pools for component

        //std::vector<ComponentMask> m_system_component_mask;                 // System component mask
        //std::vector<std::unique_ptr<ISystem>> m_systems;                     // Systems
    };

    /*
     * Template functions Implementation
     */

    /*
     * Components
     */
    template<typename C>
    void ECS::registerComponent() {
        const auto family = getComponentFamily<C>();

        if (m_component_pools.size() <= family) {
            m_component_pools.resize(family + 1, nullptr);
        }

        if (!m_component_pools[family]) {
            auto pool = new ObjectPool<C>;
            pool->expand(m_entity_index_counter);
            m_component_pools[family] = std::move(pool);
        }
    }


    // Assign new Component
    template<typename C, typename ... Args>
    void ECS::assignComponent(Entity::ID t_id, Args && ... args) {
        const auto family = getComponentFamily<C>();
        assert(!m_entity_component_mask[t_id.index()].test(family));

        auto pool = getComponentPool<C>();
        ::new(pool->get(t_id.index())) C{args ...};

        m_entity_component_mask[t_id.index()].set(family);
    }

    // Remove Component
    template<typename C>
    void ECS::removeComponent(Entity::ID t_id) {
        removeComponent(t_id, getComponentFamily<C>());
    }

    // Get Component Pool
    template<typename C>
    ObjectPool<C>* ECS::getComponentPool() const {
        const auto family = getComponentFamily<C>();

        // already registered?
        assert(m_component_pools.size() > family);
        return dynamic_cast<ObjectPool<C>*>(m_component_pools[family]);
    }

    template<typename C>
    C &ECS::getComponent(Entity::ID t_id) const {
        const auto family = getComponentFamily<C>();

        assert(m_entity_component_mask[t_id.index()].test(family));

        auto pool = getComponentPool<C>();
        auto component = pool->get_object(t_id.index());

        return *component;
    }

    // Get Mask from Template
    // todo avoid global
    /*
    template <typename C, typename ... Cargs>
    ECS::ComponentMask getMask()
    {
        return getMask<C>() |= getMask<Cargs ...>();
    };

    template <typename C>
    ECS::ComponentMask getMask()
    {
        return ECS::ComponentMask().set(getComponentFamily<C>());
    };
    */
}

#endif //PANNOIREENGINE_ECS_H
