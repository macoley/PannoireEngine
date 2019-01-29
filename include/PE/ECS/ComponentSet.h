#ifndef PE_ECS_COMPONENTSET_H
#define PE_ECS_COMPONENTSET_H

#include <vector>
#include <cassert>

#include "Defines.h"
#include "Entity.h"
#include "PE/Utils/Utils.h"

namespace PE::ECS {

    class BaseSet {
    protected:
        using base = BaseSet;

        using DataIndex = std::size_t;
        enum : DataIndex {
            VALID_ENTITY   =  DataIndex(0),
            INVALID_ENTITY = ~DataIndex(0)
        };

        using DataIndexVector   = std::vector<DataIndex>;
        using EntityVector      = std::vector<Entity>;
        using Iterator          = EntityVector::iterator;
        using CIterator         = EntityVector::const_iterator;

        DataIndexVector   m_reverse;  // like proxy entry --> component (can be invalid)
        EntityVector      m_direct;   // for iterate, list of entries

    public:
        BaseSet() = default;
        virtual ~BaseSet() = default;

        virtual void destroy(Entity);

        virtual void add(Entity);

        inline Iterator begin() {
            return m_direct.begin();
        }

        inline Iterator end() {
            return m_direct.end();
        }

        inline CIterator cbegin() const {
            return m_direct.cbegin();
        }

        inline CIterator cend() const {
            return m_direct.cend();
        }

        inline std::size_t size() const {
            return m_direct.size();
        }

        inline bool has(Entity entity) const {
            auto index = getIndex(entity);
            return index < m_reverse.size() && m_reverse[index] != INVALID_ENTITY;
        };

        // unsecure
        inline bool fast(Entity entity) const {
            return m_reverse[getIndex(entity)] != INVALID_ENTITY;
        };

    };

    /**
     * Component Set
     * @tparam C
     */
    template<typename C>
    class ComponentSet : public BaseSet {
    protected:
        using ComponentVector   = std::vector<C>;

        ComponentVector   m_data;

    public:
        ComponentSet() = default;
        virtual ~ComponentSet() = default;

        inline C &get(Entity entity) {
            assert(has(entity));
            return m_data[m_reverse[getIndex(entity)]];
        }

        inline C &getUnsafe(uint32_t index) {
            return m_data[m_reverse[index]];
        }

        // Empty args override old (i hope so)
        template<typename... Args>
        void add(Entity, Args &&...);

        void destroy(Entity) override;
    };


    /**
     * Destroying entity
     * @tparam C
     * @param index
     */
    template<typename C>
    void ComponentSet<C>::destroy(Entity entity) {
        assert(has(entity));

        auto dataIndex = m_reverse[getIndex(entity)];

        auto tmp = std::move(m_data.back());
        m_data[dataIndex] = std::move(tmp);
        m_data.pop_back();

        // direct and data have the same algorithm => only pushing last element into just deleted
        base::destroy(entity);
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

        base::add(entity);

        auto entityIndex = getIndex(entity);

        std::size_t dataIndex = m_data.size();
        m_data.push_back({std::forward<Args>(args) ...});
        m_reverse[entityIndex] = dataIndex;
    }




}


#endif //PE_ECS_COMPONENTSET_H
