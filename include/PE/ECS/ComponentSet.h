#ifndef PE_ECS_COMPONENTSET_H
#define PE_ECS_COMPONENTSET_H

#include <vector>
#include <cassert>

#include "Defines.h"

namespace PE::ECS {

    struct BaseComponentSet {
        virtual void destroy(EntityIndex) = 0;
    };

    /**
     * Component Set
     * @tparam C
     */
    template<typename C>
    class ComponentSet : public BaseComponentSet {
        using DataIndex = std::size_t;
        enum : DataIndex { INVALID_ENTITY = ~DataIndex(0) };

    public:
        ComponentSet() {};
        virtual ~ComponentSet() = default;

        C &get(EntityIndex) const;

        template<typename ... Args>
        void add(EntityIndex, Args && ... args);

        void destroy(EntityIndex) override;

    private:
        inline bool has(EntityIndex) const;

        std::vector<C> m_data;
        std::vector<DataIndex> m_reverse;  // like proxy entry --> component (can be invalid)
        std::vector<EntityIndex> m_direct; // for iterate, list of entries //todo swap on Entity not EntityIndex?
    };

    /**
     * Getting Component
     * @tparam C
     * @param index
     * @return
     */
    template<typename C>
    C &ComponentSet<C>::get(EntityIndex index) const {
        assert(has(index));

        return m_data[m_reverse[index] - 1]; // because 0 is invalid
    }

    /**
     * Checking if entity exists
     * @tparam C
     * @param index
     * @return
     */
    template<typename C>
    bool ComponentSet<C>::has(EntityIndex index) const {
        return index < m_reverse.size() && m_reverse[index] != INVALID_ENTITY;
    }


    /**
     * Destroying entity
     * @tparam C
     * @param index
     */
    template<typename C>
    void ComponentSet<C>::destroy(EntityIndex entityIndex) {
        assert(has(entityIndex));

        auto dataIndex = m_reverse[entityIndex];

        // direct and data have the same algorithm => only pushing last element into just deleted

        auto tmp = std::move(m_data.back());
        m_data[dataIndex] = std::move(tmp);
        m_data.pop_back();

        const auto back = m_direct.back();
        auto &candidate = m_reverse[entityIndex];
        m_reverse[back] = candidate;
        m_direct[candidate] = back;
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
    void ComponentSet<C>::add(EntityIndex entityIndex, Args &&... args) {
        assert(!has(entityIndex));

        if (entityIndex >= m_reverse.size()) {
            m_reverse.resize(entityIndex + 1, INVALID_ENTITY);
        }

        std::size_t dataIndex = m_data.size();
        m_data.emplace_back(std::forward<Args>(args) ...);
        m_reverse[entityIndex] = dataIndex;
        m_direct.push_back(entityIndex);
    }


}


#endif //PE_ECS_COMPONENTSET_H
