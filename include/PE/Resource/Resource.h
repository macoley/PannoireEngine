#ifndef PANNOIREENGINE_RESOURCE_H
#define PANNOIREENGINE_RESOURCE_H

#include <string>

namespace PE::Resource {

    class LoadableResource {
    public:
        LoadableResource() = default;
        virtual ~LoadableResource() = default;

        virtual void load(const std::string & path) = 0;
    };

}

#endif //PANNOIREENGINE_RESOURCE_H
