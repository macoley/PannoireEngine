#ifndef PE_ECS_VIEW_H
#define PE_ECS_VIEW_H

#include <memory>
#include <tuple>
#include <functional>

#include "PE/Utils/Utils.h"

#include "Defines.h"
#include "ComponentSet.h"

namespace PE::ECS {

    /**
     * View class
     * @tparam C
     */
    template <typename ... Comp>
    class View {
        template <typename T>
        using ComponentSetPtr = std::shared_ptr<ComponentSet<T>>;
        using BaseSetPtr      = std::shared_ptr<BaseSet>;

        using ComponentsTuple = std::tuple<ComponentSetPtr<Comp> ...>;
        using ComponentsArray = std::array<BaseSetPtr, sizeof...(Comp)>;

        using Function = std::function<void(const Entity, Comp& ...)>;

    public:
        explicit View(ComponentSetPtr<Comp> ... pools)
                :   m_base_pools{std::static_pointer_cast<BaseSet>(pools) ...},
                    m_pools(pools ...)
        {
            static_assert(sizeof...(Comp) > 0);

            std::sort(m_base_pools.begin(), m_base_pools.end(), [](const auto& a, const auto& b) {
                return a->size() < b->size();
            });

            Utils::log("Creating view with size: " + std::to_string(sizeof...(Comp)));

        };


        void each(Function fnc)
        {
            BaseSetPtr smallestPool = m_base_pools[0];

            for(auto i = smallestPool->begin(); i != smallestPool->end(); i++)
            {

            }

        }


    private:
        ComponentsArray m_base_pools;
        ComponentsTuple m_pools;
    };

}

#endif //PE_ECS_VIEW_H
