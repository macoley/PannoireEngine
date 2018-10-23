#ifndef PANNOIREENGINE_RES_UTILS_H
#define PANNOIREENGINE_RES_UTILS_H

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <utility>

namespace PE::Resource {

    /**
     * ID resource
     */
    using resourceIndex = std::size_t;

    /**
     * Consts ID
     */
    enum : resourceIndex {
        INVALID_INDEX = 0
    };

    /**
     * Resources Path
     */
    constexpr const char * resourceDir = "resources";

    /**
     * Extension
     */
    constexpr const char * resourceExtension = "res";
}

#endif //PANNOIREENGINE_RES_UTILS_H
