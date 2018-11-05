#ifndef PE_ECS_COMPONENTSET_H
#define PE_ECS_COMPONENTSET_H

#include <vector>
#include <cassert>

#include "Defines.h"
#include "Entity.h"

namespace PE::ECS {

    struct BaseComponentSet {
        virtual void destroy(Entity) = 0;
    };

    /**
     * Component Set
     * @tparam C
     */
    template<typename C>
    class ComponentSet : public BaseComponentSet {
        using DataIndex = std::size_t;
        enum : DataIndex { INVALID_ENTITY = ~DataIndex(0) };

        using ComponentVector   = std::vector<C>;
        using DataIndexVector   = std::vector<DataIndex>;
        using EntityVector = std::vector<Entity>;

        using Iterator = EntityVector::iterator;

    public:
        ComponentSet() {};
        virtual ~ComponentSet() = default;

        C &get(Entity);

        template<typename ... Args>
        void add(Entity, Args && ... args);

        void destroy(Entity) override;

        inline Iterator begin() {
            return m_direct.begin();
        }

        inline Iterator end() {
            return m_direct.end();
        }

        inline std::size_t size() {
            return m_direct.size();
        }

        inline bool has(Entity) const;

    private:
        ComponentVector   m_data;
        DataIndexVector   m_reverse;  // like proxy entry --> component (can be invalid)
        EntityVector      m_direct;   // for iterate, list of entries
    };

    /**
     * Getting Component
     * @tparam C
     * @param index
     * @return
     */
    template<typename C>
    C &ComponentSet<C>::get(Entity entity) {
        assert(has(entity));

        return m_data[m_reverse[getIndex(entity)]]; // because 0 is invalid
    }

    /**
     * Checking if entity exists
     * @tparam C
     * @param index
     * @return
     */
    template<typename C>
    bool ComponentSet<C>::has(Entity entity) const {
        auto index = getIndex(entity);
        return index < m_reverse.size() && m_reverse[index] != INVALID_ENTITY;
    }


    /**
     * Destroying entity
     * @tparam C
     * @param index
     */
    template<typename C>
    void ComponentSet<C>::destroy(Entity entity) {
        assert(has(entity));

        auto entityIndex = getIndex(entity);
        auto dataIndex = m_reverse[entityIndex];

        // direct and data have the same algorithm => only pushing last element into just deleted

        auto tmp = std::move(m_data.back());
        m_data[dataIndex] = std::move(tmp);
        m_data.pop_back();

        const auto back = m_direct.back();          // entity
        auto &candidate = m_reverse[entityIndex];   // dataIndex
        m_reverse[getIndex(back)] = candidate;      // reverse[entity] = dataIndex
        m_direct[candidate] = back;                 // direct[dataIndex] = entity
        candidate = INVALID_ENTITY;
        m_direct.pop_back();

    }

    /**
     * Adding Entity
     * @tparam C
     * @tparam Args
     * @param index
     * @param args
     */
    template<typename C>
    template<typename... Args>
    void ComponentSet<C>::add(Entity entity, Args &&... args) {
        assert(!has(entity));

        auto entityIndex = getIndex(entity);

        if (entityIndex >= m_reverse.size()) {
            m_reverse.resize(entityIndex + 1, INVALID_ENTITY);
        }

        std::size_t dataIndex = m_data.size();
        //m_data.emplace_back(std::forward<Args>(args) ...); // or below
        m_data.push_back({std::forward<Args>(args) ...});
        m_reverse[entityIndex] = dataIndex;
        m_direct.push_back(entity);
    }


}


#endif //PE_ECS_COMPONENTSET_H
