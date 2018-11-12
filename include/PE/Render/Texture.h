#ifndef PANNOIREENGINE_TEXTURE_H
#define PANNOIREENGINE_TEXTURE_H

#include <cstdint>
#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gsl/gsl>
#include <stb_image.h>

#include "PE/Resource/Resource.h"
#include "PE/Utils/Utils.h"

namespace PE::Render {

    class Texture : public Resource::IResource {
    public:
        explicit Texture() = default;
        virtual ~Texture();

        void load(const std::string & path) override;

    private:
        void loadImageFromFile(const std::string& path);

        uint32_t m_id; //GLuint is 32bit
        int32_t m_width;
        int32_t m_height;
        int32_t m_components;
    };


    /**
    inline void bindTexture() const {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    inline void bindTextureUnit(uint8_t index) const {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    inline void bindTextureUnitToShader(const Shader &shader, const std::string& name, uint8_t index) const {
        glActiveTexture(GL_TEXTURE0 + index);

        shader.use();
        shader.setInt(name, index);

        glBindTexture(GL_TEXTURE_2D, id);
    }
    */

}

#endif //PANNOIREENGINE_TEXTURE_H
