#ifndef PANNOIREENGINE_ENGINE_COMPONENT_H
#define PANNOIREENGINE_ENGINE_COMPONENT_H

#include <cstdint>

namespace PE::Engine::Component {

    struct Transform {
        float xPos = .0f;
        float yPos = .0f;
        float zPos = .0f;

        float xScale = 1.0f;
        float yScale = 1.0f;
        float zScale = 1.0f;

        float xAngle = .0f;
        float yAngle = .0f;
        float zAngle = .0f;
    };

    struct Model {
        uint32_t resIndex;
    };

    struct Camera {
        float zoom = 45.0f;
    };

}

#endif //PANNOIREENGINE_ENGINE_COMPONENT_H
