#ifndef PANNOIREENGINE_ECS_H
#define PANNOIREENGINE_ECS_H

#include <memory>
#include <vector>
#include <type_traits>
#include <iostream>
#include <utility>
#include <functional>

#include "PE/Engine/Common.h"
#include "PE/Engine/Utils.h"

#include "PE/Engine/Pool.h"
#include "PE/Engine/Entity.h"
#include "PE/Engine/Component.h"
#include "PE/Engine/System.h"

#include "PE/Engine/ComponentType.h"

namespace PE::Engine {

    class Entity;

    class BaseSystem;

    template <typename ... Components>
    class System;


    /**
     * ECS Architecture Manager
     * Please create with shared_ptr
     */
    class ECS : public std::enable_shared_from_this<ECS> {
    public:
        /*
         * Entities
         */
        Entity createEntity();

        void destroyEntity(Entity::ID t_id);

        Entity getEntity(Entity::ID t_id);

        /*
         * Components
         */

        template <typename C, typename ... Args>
        void assignComponent(Entity::ID t_id, Args && ... args);

        template<typename C>
        void removeComponent(Entity::ID t_id);

        template <typename C>
        C& getComponent(Entity::ID t_id) const;

        template<typename C>
        void registerComponent();

        /*
         * Systems
         */
        template <typename S>
        void createSystem();

        template<typename S>
        void updateSystem();

    private:
        /*
         * Entities Private functions
         */
        void accomodateEntity(uint32_t t_index);

        /*
         * Component
         */

        template <typename C>
        ObjectPool<C>* getComponentPool() const;

        void removeComponent(Entity::ID t_id, FamilyIndex t_family);

        /*
         * System
         */
        template <typename ... Cargs>
        void unpackSystem(System<Cargs ...>* system);

        /*
         * System Pack
         */
        template <typename ... Cargs>
        class Pack {
            typedef std::tuple<Cargs * ...> TuplePack;

        public:
            Pack(ECS * t_parent) : m_parent(t_parent) {
                setPool<Cargs ...>();
            };

            inline TuplePack getArgs() {
                return m_args;
            }

        private:
            template <typename Single>
            void setPool()
            {
                std::get<Single*>(m_args) = m_parent->getComponentPool<Single>()->get_objects();
            };

            template <typename First, typename Second, typename ... Rest>
            void setPool()
            {
                setPool<First>();
                setPool<Second, Rest ...>();
            };

            ECS * m_parent;
            TuplePack m_args;
        };

        template <typename ... Cargs>
        friend class Pack;

        /**
         * Main ECS Data
         */
        uint32_t m_entity_index_counter = 0;                                // Index counter
        std::vector<uint32_t> m_free_entity_list;                           // List of free indexes
        std::vector<uint32_t> m_entity_version;                             // Versions of entity

        std::vector<ComponentMask> m_entity_component_mask;                 // Mask of components for each entity
        std::vector<MemoryPool*> m_component_pools;                         // Memory pools for component

        std::vector<ComponentMask> m_system_component_mask;                 // System component mask
        std::vector<BaseSystem*> m_systems;                                 // Systems
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

        std::cout << "Registering: " << typeid(C).name() << "\n";

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
        const auto originalMask = m_entity_component_mask[t_id.index()];

        if (m_component_pools.size() <= family) {
            registerComponent<C>();
        }

        // if is already
        assert(!originalMask.test(family));

        auto pool = getComponentPool<C>();
        ::new(pool->get(t_id.index())) C{std::forward<Args>(args) ...};

        m_entity_component_mask[t_id.index()].set(family);

        // Add to system cache
        for (std::size_t system_index = 0; system_index < m_system_component_mask.size(); ++system_index) {
            if(
                    (originalMask & m_system_component_mask[system_index]) != m_system_component_mask[system_index] &&
                    (m_entity_component_mask[t_id.index()] & m_system_component_mask[system_index]) == m_system_component_mask[system_index]
            )
            {
                m_systems[system_index]->addEntity(t_id.index());
            }
        }
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

    /*
     * Systems
     */
    template <typename S>
    void ECS::createSystem() {
        static_assert(std::is_base_of<BaseSystem, S>::value, "System must be inherited from System");

        // Todo search all entries which matched to this component mask and add them

        const auto index = S::getSystemIndex();
        auto system = new S();

        if (m_systems.size() <= index) {
            m_systems.resize(index + 1, nullptr);
            m_system_component_mask.resize(index + 1);
        }

        m_systems[index] = reinterpret_cast<BaseSystem *>(system);
        m_system_component_mask[index] = system->m_component_mask;
    }

    template<typename S>
    void ECS::updateSystem() {
        const auto index = S::getSystemIndex();
        S* system = reinterpret_cast<S*>(m_systems[index]);

        unpackSystem(system);
    }


    template <typename ... Cargs>
    void ECS::unpackSystem(System<Cargs ...>* system)
    {
        auto pack = Pack<Cargs ...>(this);

        std::invoke(&System<Cargs ...>::update, system, std::get<Cargs*>(pack.getArgs())...);
    }


    /*
     * Implemented Entity Wrappers
     */

    template<typename C, typename... Args>
    void Entity::assignComponent(Args &&... args) {
        m_manager_ptr->assignComponent<C>(m_id, std::forward<Args>(args) ...);
    }

    template<typename C>
    void Entity::removeComponent() {
        m_manager_ptr->removeComponent<C>(m_id);
    }

    template<typename C>
    C &Entity::getComponent() const {
        return m_manager_ptr->getComponent<C>(m_id);
    }

}

#endif //PANNOIREENGINE_ECS_H
