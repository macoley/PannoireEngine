#ifndef PANNOIREENGINE_RESOURCEHANDLE_H
#define PANNOIREENGINE_RESOURCEHANDLE_H

#include <cstdint>

namespace PE::Resource {

    template <typename Resource>
    class ResourceHandle {
        friend class ResourceManager;
    public:
        // There is no reference incrementing
        ResourceHandle() : m_version(0) {}

        ResourceHandle(const ResourceHandle<Resource>& t_other)
                : m_index(t_other.m_index),
                  m_version(t_other.m_version)
        {
            // Todo manager ref increment
        }

        ~ResourceHandle() {
            // Todo manager ref decrement
        }

        const Resource &get() const {
            // Todo manager get resource
        }

        Resource &get() {
            // Todo manager get resource
        }

    private:
        ResourceHandle(uint16_t t_index, uint16_t t_version, ResourceHandle* t_manager_ptr)
                : m_index(t_index),
                  m_version(t_version),
                  m_manager_ptr(t_manager_ptr)
        {
            // Todo manager ref increment
        }

        uint16_t m_index;
        uint16_t m_version;    // 0 equals invalid
        ResourceHandle *m_manager_ptr;
    };

}

#endif //PANNOIREENGINE_RESOURCEHANDLE_H
