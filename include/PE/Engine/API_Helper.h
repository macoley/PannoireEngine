#ifndef PANNOIREENGINE_API_HELPER_H
#define PANNOIREENGINE_API_HELPER_H

template <typename T>
struct ComponentBind {
    explicit ComponentBind(T& t_component) : m_component(t_component) {};
    ComponentBind(T &&) = delete;
    ComponentBind() = delete;
    T& m_component;
};

#define getter_setter(type, name)                                          \
    type get_##name() { return m_component.name; }                         \
    void set_##name(type t_##name) { m_component.name = t_##name; }

#define comp_wrapper_begin(ComponentType)                                                \
template <>                                                                              \
struct ComponentBind<ComponentType> {                                                    \
    explicit ComponentBind(ComponentType& t_component) : m_component(t_component) {};    \
    ComponentBind(ComponentType &&) = delete;                                            \
    ComponentBind() = delete;                                                            \
    ComponentType& m_component;

#define comp_wrapper_end()                                                               \
};

#define reg_component(ComponentType)                                                     \
m_engine->registerConstructor<ComponentBind<ComponentType>, ComponentType&>(#ComponentType);

#define reg_component_val(ComponentType, Value)                                          \
m_engine->registerProperty(&ComponentBind<ComponentType>::get_##Value, &ComponentBind<ComponentType>::set_##Value, #Value);


#endif //PANNOIREENGINE_API_HELPER_H
