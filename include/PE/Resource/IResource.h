#ifndef PE_RES_IRESOURCE_H
#define PE_RES_IRESOURCE_H

#include <string>

namespace PE::Resource {

    /**
     * Loadable Resource Interface
     */
    class IResource {
    public:
        virtual void load(const std::string & path) = 0;
    };

}

#endif //PE_RES_IRESOURCE_H
