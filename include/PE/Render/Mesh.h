#ifndef PANNOIREENGINE_MESH_H
#define PANNOIREENGINE_MESH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PE/Render/Shader.h"
#include "PE/Render/Defines.h"

namespace PE::Render {

    /**
     * Mesh class
     */
    class Mesh {
    public:
        Mesh(std::vector<Vertex> t_vertices, std::vector<unsigned int> t_indices);
        virtual ~Mesh();

        inline void draw(Shader & t_shader) {
            glBindVertexArray(m_VAO);
            glDrawElements(GL_TRIANGLES, (GLsizei) m_indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

    private:
        void setupMesh();

        unsigned int m_VAO, m_VBO, m_EBO;

        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
    };
}


#endif //PANNOIREENGINE_MESH_H
