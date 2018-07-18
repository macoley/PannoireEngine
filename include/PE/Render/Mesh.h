#ifndef PANNOIREENGINE_MESH_H
#define PANNOIREENGINE_MESH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PE/Render/Shader.h"

namespace PE::Render {

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh {
    public:
        Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indices, std::vector<Texture> t_textures);
        virtual ~Mesh();

        inline void draw(Shader & t_shader) {

            // bind appropriate textures
            unsigned int diffuseNr  = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr   = 1;
            unsigned int heightNr   = 1;
            unsigned int opacityNr   = 1;

            unsigned int i = 0;

            for(; i < m_textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)
                std::string number;
                std::string name = m_textures[i].type;

                if(name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if(name == "texture_specular")
                    number = std::to_string(specularNr++); // transfer unsigned int to stream
                else if(name == "texture_normal")
                    number = std::to_string(normalNr++); // transfer unsigned int to stream
                else if(name == "texture_height")
                    number = std::to_string(heightNr++); // transfer unsigned int to stream
                else if(name == "texture_opacity")
                    number = std::to_string(opacityNr++); // transfer unsigned int to stream

                // now set the sampler to the correct texture unit
                glUniform1i(glGetUniformLocation(t_shader.getProgramID(), (name + number).c_str()), i);
                // and finally bind the texture
                glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
            }

            if(opacityNr == 1) {
                glActiveTexture(GL_TEXTURE0 + i);
                glUniform1i(glGetUniformLocation(t_shader.getProgramID(), "texture_opacity1"), i);
                glBindTexture(GL_TEXTURE_2D, whiteTexture);
                i++;
            }

            // Render

            glBindVertexArray(m_VAO);
            glDrawElements(GL_TRIANGLES, (GLsizei) m_indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            glActiveTexture(GL_TEXTURE0);
        }

    private:
        void setupMesh();

        unsigned int m_VAO, m_VBO, m_EBO;

        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<Texture> m_textures;

        static unsigned int transparentTexture;
        static unsigned int whiteTexture;
    };
}


#endif //PANNOIREENGINE_MESH_H
