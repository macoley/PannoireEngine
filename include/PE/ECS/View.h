#ifndef PE_ECS_VIEW_H
#define PE_ECS_VIEW_H

#include <memory>
#include <tuple>
#include <functional>
#include <utility>

#include "PE/Utils/Utils.h"

#include "Defines.h"
#include "ComponentSet.h"

namespace PE::ECS {

    /**
     * View class
     * @tparam C
     */
    template<typename ... Comp>
    class View {
        template<typename T>
        using ComponentSetPtr = std::shared_ptr<ComponentSet<T>>;
        using BaseSetRawPtr   = BaseSet *;

        using ComponentsTuple = std::tuple<ComponentSetPtr<Comp> ...>;
        using ComponentsArray = std::array<BaseSetRawPtr, sizeof...(Comp)>;

        using Indices = std::make_index_sequence<sizeof...(Comp)>;
        using Function = std::function<void(const Entity, Comp &...)>;

    public:
        explicit View(ComponentSetPtr<Comp> ... pools)
                : m_pools(pools ...) {
            static_assert(sizeof...(Comp) > 0);
        };

        void each(Function fnc) {
            iterate(fnc, Indices{});
        }

    private:
        template<std::size_t... I>
        void iterate(Function fnc, std::index_sequence<I...>);

        ComponentsTuple m_pools;
    };


    /**
     * Iterate function
     * @tparam Comp
     * @tparam I
     * @param fnc
     */
    template<typename... Comp>
    template<size_t... I>
    void View<Comp...>::iterate(View::Function fnc, std::index_sequence<I...>) {
        ComponentsArray components{
                static_cast<BaseSetRawPtr>(std::get<I>(m_pools).get()) ...
        };

        using SetPtr = const BaseSetRawPtr;

        // Sort it
        if constexpr(sizeof...(Comp) > 1) {
            std::sort(components.begin(), components.end(), [](SetPtr a, SetPtr b) {
                return a->size() < b->size();
            });
        }

        SetPtr min = components[0];

        // Iterate trough all entities
        for (auto e = min->begin(); e < min->end(); e++) {
            if constexpr(sizeof...(Comp) > 1) {
                if (std::all_of(components.cbegin() + 1, components.cend(), [e](SetPtr set) {
                    return set->fast(*e);
                })) {
                    fnc(*e, std::get<I>(m_pools)->get(*e) ...);
                }
            } else {
                fnc(*e, std::get<I>(m_pools)->get(*e) ...);
            }
        }
    }

}

#endif //PE_ECS_VIEW_H
