
#include <PE/Render/Mesh.h>

#include "PE/Render/Mesh.h"

namespace PE::Render {

    Mesh::Mesh(std::vector<Vertex> t_vertices, std::vector<uint32_t> t_indices)
        : m_vertices(std::move(t_vertices)), m_indices(std::move(t_indices))
    {
        setupMesh();
        calcCenter();
    }

    void Mesh::setupMesh() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t), &m_indices[0], GL_STATIC_DRAW);

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
        glBindVertexArray(0);

        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void Mesh::calcCenter() {
        assert(!m_vertices.empty());

        float min_x, max_x, min_y, max_y, min_z, max_z;

        min_x = max_x = m_vertices[0].Position.x;
        min_y = max_y = m_vertices[0].Position.y;
        min_z = max_z = m_vertices[0].Position.z;

        for(const Vertex &v : m_vertices)
        {
            if(v.Position.x > max_x)
                max_x = v.Position.x;
            else if(v.Position.x < min_x)
                min_x = v.Position.x;

            if(v.Position.y > max_y)
                max_y = v.Position.y;
            else if(v.Position.y < min_y)
                min_y = v.Position.y;

            if(v.Position.z > max_z)
                max_z = v.Position.z;
            else if(v.Position.z < min_z)
                min_z = v.Position.z;
        }

        m_center = glm::vec3((min_x+max_x)/2, (min_y+max_y)/2, (min_z+max_z)/2);
    }

    glm::vec3 Mesh::getNearest(glm::vec3 point) {
        auto min = std::min_element(std::begin(m_vertices), std::end(m_vertices), [&](const Vertex &a, const Vertex &b) {
            return glm::length(a.Position - point) < glm::length(b.Position - point);
        });

        return min->Position;
    }
}