#ifndef PE_RES_IRESOURCE_H
#define PE_RES_IRESOURCE_H

#include <string>

namespace PE::Resource {

    class Counter {
    protected:
        using FamilyCounter = std::size_t;
    public:
        static FamilyCounter m_i;
    };

    /**
     * Loadable Resource Interface
     */
    template<typename Resource>
    class IResource : public Counter {

    public:
        void load(std::string && path) {
            static_cast<Resource&>(*this)->load(std::forward<decltype(path)>(path));
        };

        static Counter::FamilyCounter id() {
            static Counter::FamilyCounter family = Counter::m_i++;
            return family;
        }
    };




}

#endif //PE_RES_IRESOURCE_H
