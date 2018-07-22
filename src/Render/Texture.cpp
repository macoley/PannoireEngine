#include "PE/Render/Texture.h"

namespace PE::Render {

    Texture::Texture(const Texture::byte *data, uint8_t components, Texture::size width, Texture::size height) {
        // Generate texture
        glGenTextures(1, &id);

        // Choose format
        GLenum format;
        if (components == 1)
            format = GL_RED;
        else if (components == 3)
            format = GL_RGB;
        else if (components == 4)
            format = GL_RGBA;

        // Bind texture and move it to VRAM
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set repeat options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &id);
    }
}