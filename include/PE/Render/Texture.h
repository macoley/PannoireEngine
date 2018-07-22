#ifndef PANNOIREENGINE_TEXTURE_H
#define PANNOIREENGINE_TEXTURE_H

#include <cstdint>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "PE/Render/Shader.h"

namespace PE::Render {

    /**
     * Texture Resource
     */
    class Texture {
        using byte      = unsigned char;
        using textureID = uint32_t; // GLuint is 32bit
        using size      = uint32_t;

    public:
        Texture(const byte* data, uint8_t components, size width, size height);
        virtual ~Texture();

        inline textureID getID() const {
            return id;
        }

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

    private:
        textureID id;
    };

}

#endif //PANNOIREENGINE_TEXTURE_H
