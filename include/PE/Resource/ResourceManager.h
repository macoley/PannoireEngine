#ifndef PANNOIREENGINE_RESOURCEMANAGER_H
#define PANNOIREENGINE_RESOURCEMANAGER_H

#include <vector>
#include <string>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <unordered_map>

#include "PE/Resource/Utils.h"
#include "PE/Resource/ResourceData.h"
#include "PE/Resource/ResourceHandle.h"

namespace PE::Resource {

    template <typename R>
    class ResourceHandle;

    /**
     * Resource manager
     */
    class ResourceManager {
        template <typename R> using Handle = ResourceHandle<R>;

    public:
        ResourceManager() = default;
        virtual ~ResourceManager() = default;

        template <typename R>
        Handle<R> get(const std::string&);


    private:

    };

    template<typename R>
    ResourceManager::Handle<R> ResourceManager::get(const std::string &) {
        return ResourceManager::Handle<R>();
    }


}

#endif //PANNOIREENGINE_RESOURCEMANAGER_H
