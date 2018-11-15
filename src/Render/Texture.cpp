
#include "PE/Render/Texture.h"
#include <stb_image.h>

namespace PE::Render {

    void Texture::loadImageFromFile(const std::string &path) {
        unsigned char* data;

        data = stbi_load(path.c_str(), &m_width, &m_height, &m_components, 0);

        if (!data) {
            Utils::log("Texture failed to load at path: " + path);
        }

        // Generate texture
        glGenTextures(1, &m_id);

        // Choose format
        GLenum format = GL_NONE;
        if (m_components == 1)
            format = GL_RED;
        else if (m_components == 3)
            format = GL_RGB;
        else if (m_components == 4)
            format = GL_RGBA;

        // Bind texture and move it to VRAM
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set repeat options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_id);
    }

    void Texture::load(const std::string &path) {
        loadImageFromFile(path);
    }
}