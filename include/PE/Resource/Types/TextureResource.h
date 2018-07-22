#ifndef PANNOIREENGINE_TEXTURETYPE_H
#define PANNOIREENGINE_TEXTURETYPE_H

#include <iostream>

#include "PE/Resource/Types/BaseResource.h"
#include "PE/Render/Texture.h"

namespace PE::Resource {

    using base = BaseResource<const unsigned char*, uint8_t, uint32_t, uint32_t>;

    /**
     * Texture Resource
     */
    template <>
    class Resource<PE::Render::Texture> : public base
    {
    public:
        Resource() {
            std::cout << "Texture \n";
        }

        base::constructorArgs get() override {
            return base::constructorArgs(data, components, width, height);
        }

    private:
        const unsigned char* data;
        uint8_t              components;
        uint32_t             width;
        uint32_t             height;
    };

}

#endif //PANNOIREENGINE_TEXTURETYPE_H
