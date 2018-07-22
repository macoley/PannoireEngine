#ifndef PANNOIREENGINE_RESOURCEDATA_H
#define PANNOIREENGINE_RESOURCEDATA_H

#include <cassert>
#include <tuple>

#include "PE/Render/Texture.h"

namespace PE::Resource {

    /**
     * Shader Data
     */
    struct ShaderData {
        std::string vertexShader;
        std::string fragmentShader;
    };

    /**
     * Texture Data
     */
    struct TextureData {
        unsigned char*  data;
        uint8_t         components;
        uint32_t        width;
        uint32_t        height;
    };


    template <typename R>
    struct resource {
        auto get() {
            return std::make_tuple();
        }
    };

    template <>
    struct resource<PE::Render::Texture> {
        unsigned char*  data;
        uint8_t         components;
        uint32_t        width;
        uint32_t        height;

        auto get() {
            return std::make_tuple(data, components, width, height);
        }
    };
}

#endif //PANNOIREENGINE_RESOURCEDATA_H
