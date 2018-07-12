#ifndef PANNOIREENGINE_ECS_H
#define PANNOIREENGINE_ECS_H

#include <iostream>
#include <type_traits>
#include <memory>
#include <bitset>
#include <vector>
#include <gsl/gsl>
#include <iterator>

#include "PE/Render/Render.h"
#include "PE/Engine/Pool.h"

namespace PE::Engine {

    /*
    class EntityIterator
            : public std::iterator<std::input_iterator_tag, ID>
    {
    public:
        EntityIterator(std::shared_ptr<ECS> t_ecs, uint32_t index)
                : i(index), m_manager_ptr(std::move(t_ecs)), m_capacity(m_manager_ptr->getCapacity())
        {
            std::sort(m_manager_ptr->m_free_entity_list.begin(), m_manager_ptr->m_free_entity_list.end());
            m_free_cursor = 0;
        }

        template <typename ... Cargs>
        EntityIterator(std::shared_ptr<ECS> t_ecs, uint32_t index)
                : i(index), m_manager_ptr(std::move(t_ecs)), m_mask(getMask<Cargs ...>()), m_capacity(m_manager_ptr->getCapacity())
        {
            std::sort(m_manager_ptr->m_free_entity_list.begin(), m_manager_ptr->m_free_entity_list.end());
            m_free_cursor = 0;
        }

        EntityIterator& operator++() {
            next();
            return *this;
        }

        bool operator == (const EntityIterator & other) const {
            return i == other.i;
        }

        bool operator != (const EntityIterator & other) const {
            return i != other.i ;
        }

        bool operator < (const EntityIterator & other) const {
            return i < other.i;
        }

        operator

    protected:
        void next() {
            do {
                ++i;
            } while(!checkEntity() && i < m_capacity);
        }

        inline bool checkEntity()
        {
            return checkFreeList() && checkMask();
        }

        inline bool checkFreeList()
        {
            const auto & free_list = m_manager_ptr->m_free_entity_list;
            if(m_free_cursor < free_list.size() && free_list[m_free_cursor] == i)
            {
                ++m_free_cursor;
                return false;
            }
            return true;
        }

        inline bool checkMask()
        {
            return (m_manager_ptr->m_entity_component_mask[i] & m_mask) == m_mask;
        }

    private:
        uint32_t i;
        std::shared_ptr<ECS> m_manager_ptr;
        std::size_t m_capacity;
        std::size_t m_free_cursor;
        ECS::ComponentMask m_mask;
    };
    */
}

#endif //PANNOIREENGINE_ECS_H
