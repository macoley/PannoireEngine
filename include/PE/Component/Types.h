#ifndef PANNOIREENGINE_COMPONENT_TYPE_H
#define PANNOIREENGINE_COMPONENT_TYPE_H

#include <string>

namespace PE::Component {

    /**
     * Handles
     */
    using ResourceTextureID = std::string;


    /**
     * Components
     */
    struct Transform {
        float x, y, z;
    };

    struct Render {
        float temp;
    };

}

#endif //PANNOIREENGINE_COMPONENT_TYPE_H
