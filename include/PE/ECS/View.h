#ifndef PE_ECS_VIEW_H
#define PE_ECS_VIEW_H

#include <memory>
#include <tuple>

#include "PE/Utils/Utils.h"

#include "Defines.h"
#include "ComponentSet.h"

namespace PE::ECS {

    /**
     * View class
     * @tparam C
     */
    template <typename ... C>
    class View {
        template <typename T>
        using ComponentSetPtr = std::shared_ptr<ComponentSet<T>>;
        using ComponentsTuple = std::tuple<ComponentSetPtr<C> ...>;

    public:
        explicit View(ComponentSetPtr<C> ... pools)
                : m_pools(pools ...)
        {

            //Utils::log("Smallest pool: " + std::to_string(m_min_index));

        };

    private:

        // Get Mask from Template
        template <typename Single>
        std::size_t min() const
        {
            return std::get<ComponentSet<Single>>(m_pools).size();
        };

        // * must has first and second for overloading
        template <typename First, typename Second, typename ... Rest>
        std::size_t min() const
        {
            const auto first = min<First>();
            const auto rest = min<Second, Rest ...>();

            return  first < rest ? first : rest;
        };

        template <typename Candidate>
        void iterate()
        {
            if(min<Candidate> != min<C ...>)
                return;


        }

        void each()
        {



        }

        ComponentsTuple m_pools;
    };

}

#endif //PE_ECS_VIEW_H
