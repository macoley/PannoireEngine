#ifndef PE_RENDER_DEFINES_H
#define PE_RENDER_DEFINES_H

#include <glm/glm.hpp>

namespace PE::Render {

    /**
     * Main vertex properties struct
     */
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

}

#endif //PE_RENDER_DEFINES_H
