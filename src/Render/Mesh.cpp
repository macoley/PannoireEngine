
#include "PE/Render/Mesh.h"

namespace PE::Render {

    unsigned int generateTexture(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        unsigned int ID;
        GLubyte data[] = { r, g, b, a };

        glGenTextures(1, &ID);

        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        return ID;
    }

    unsigned int Mesh::transparentTexture = 0;
    unsigned int Mesh::whiteTexture = 0;

    Mesh::Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indices, std::vector<Texture> t_textures)
        : m_vertices(std::move(t_vertices)), m_indices(std::move(t_indices)), m_textures(std::move(t_textures))
    {
        if(!transparentTexture) {
            transparentTexture = generateTexture(255, 255, 255, 0);
        }

        if(!whiteTexture) {
            whiteTexture = generateTexture(255, 255, 255, 255);
        }

        setupMesh();
    }

    void Mesh::setupMesh() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        glBindVertexArray(0);
    }

    Mesh::~Mesh() {
        // Dont do it, because in other places (eg vectors) Mesh will be copied and so

        /*
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
         */
    }
}