#ifndef PANNOIREENGINE_ECS_H
#define PANNOIREENGINE_ECS_H

#include <iostream>
#include <type_traits>
#include <memory>
#include <bitset>
#include <vector>

#include "PE/Render/Render.h"
#include "PE/Engine/Pool.h"

namespace PE::Engine {

    const static std::size_t MAX_COMPONENTS = 64;

    /*
     * ID
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


    /*
     * Component Family
     */
    struct BaseComponent {
    public:
        typedef std::size_t Family;

    protected:
        static Family m_family_counter;
    };

    template<typename C>
    struct Component : public BaseComponent {
        static Family getFamily();
    };

    template<typename C>
    BaseComponent::Family Component<C>::getFamily() {
        static Family family = m_family_counter++;
        return family;
    }

    template<typename C>
    static BaseComponent::Family getFamily() {
        return Component<typename std::remove_const<C>::type>::getFamily();
    }

    /*
     * ENTITY
     */
    class ECS;
    template<typename C>
    class ComponentHandler;

    class EntityHandler {
    public:
        // Get components (tuple)
        // Add component

        EntityHandler(std::shared_ptr<ECS> t_manager_ptr, ID t_id)
                : m_manager_ptr(std::move(t_manager_ptr)), m_id(t_id) {}

        template <typename C>
        inline void removeComponent();

        inline void destroyEntity();

        template <typename C, typename ... Args>
        inline ComponentHandler<C> assignComponent(Args && ... args);

        template <typename C>
        inline ComponentHandler<C> getComponent();

    private:
        std::shared_ptr<ECS> m_manager_ptr;
        ID m_id;
    };

    /*
     * COMPONENT
     */
    template<typename C>
    class ComponentHandler {
    public:

        ComponentHandler(std::shared_ptr<ECS> t_manager_ptr, ID t_id)
            : m_manager_ptr(std::move(t_manager_ptr)), m_id(t_id) {}

        inline C& get();

        // Todo get Entity
        // Todo subscribe other <Component> in other* Entity(Handler)


    private:
        std::shared_ptr<ECS> m_manager_ptr;
        ID m_id;
    };


    /*
     * ECS
     */
    class ECS : public std::enable_shared_from_this<ECS> {
    public:
        typedef std::bitset<MAX_COMPONENTS> ComponentMask;

        template<typename C>
        void registerComponent();

        EntityHandler createEntity();

        void destroyEntity(ID t_id);

        template<typename C>
        inline void removeComponent(ID t_id);

        template <typename C, typename ... Args>
        ComponentHandler<C> assignComponent(ID t_id, Args && ... args);

        template<typename C>
        C& getComponent(ID t_id);

    private:
        void removeComponent(ID t_id, BaseComponent::Family t_family);

        void accomodateEntity(uint32_t t_index);

        template <typename C>
        inline std::shared_ptr<ObjectPool<C>> getComponentPool();

        uint32_t m_entity_index_counter = 0;
        std::vector<std::shared_ptr<MemoryPool>> m_component_pools;         // Memory pools for component
        std::vector<uint32_t> m_free_entity_list;                           // List of free indexes
        std::vector<ComponentMask> m_entity_component_mask;                 // Mask of components for each entity
        std::vector<uint32_t> m_entity_version;                             // Versions of entity
    };

    template<typename C>
    void ECS::removeComponent(ID t_id) {
        removeComponent(t_id, getFamily<C>());
    }

    template<typename C>
    void ECS::registerComponent() {
        //static_assert(std::is_base_of<ECS::Deps, D>);

        const BaseComponent::Family family = getFamily<C>();

        if (m_component_pools.size() <= family) {
            m_component_pools.resize(family + 1, nullptr);
        }

        if (!m_component_pools[family]) {
            auto pool = std::make_shared<ObjectPool<C> >();
            pool->expand(m_entity_index_counter);
            m_component_pools[family] = std::move(pool);
        }

    }

    template <typename C>
    void EntityHandler::removeComponent() {
        m_manager_ptr->removeComponent<C>(m_id);
    }

    void EntityHandler::destroyEntity() {
        m_manager_ptr->destroyEntity(m_id);
    }

    template <typename C, typename ... Args>
    ComponentHandler<C>  EntityHandler::assignComponent(Args && ... args) {
        return m_manager_ptr->assignComponent<C>(m_id, args ...);
    }

    template<typename C>
    std::shared_ptr<ObjectPool<C>> ECS::getComponentPool() {
        BaseComponent::Family family = getFamily<C>();

        // already registered?
        assert(m_component_pools.size() > family);
        return std::dynamic_pointer_cast<ObjectPool<C>>(m_component_pools[family]);
    }

    template<typename C, typename ... Args>
    ComponentHandler<C> ECS::assignComponent(ID t_id, Args && ... args) {
        BaseComponent::Family family = getFamily<C>();
        assert(!m_entity_component_mask[t_id.index()].test(family));

        auto pool = getComponentPool<C>();
        ::new(pool->get(t_id.index())) C{args ...};

        m_entity_component_mask[t_id.index()].set(family);

        return ComponentHandler<C>(shared_from_this(), t_id);
    }

    template<typename C>
    C& ECS::getComponent(ID t_id) {
        BaseComponent::Family family = getFamily<C>();
        assert(m_entity_component_mask[t_id.index()].test(family));

        auto pool = getComponentPool<C>();
        auto component = pool->get_object(t_id.index());

        return *component;
    }

    template<typename C>
    C &ComponentHandler<C>::get() {
        return m_manager_ptr->getComponent<C>(m_id);
    }

    template<typename C>
    ComponentHandler<C> EntityHandler::getComponent() {
        return ComponentHandler<C>(m_manager_ptr, m_id);
    }


}

#endif //PANNOIREENGINE_ECS_H
