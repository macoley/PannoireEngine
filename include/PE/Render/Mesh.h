#ifndef PE_RENDER_MESH_H
#define PE_RENDER_MESH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PE/Render/Shader.h"
#include "PE/Render/Defines.h"
#include "PE/Resource/Resource.h"

namespace PE::Render {

    /**
     * Mesh class
     */
    class Mesh : public Resource::IResource<Mesh> {
    public:
        Mesh(std::vector<Vertex> t_vertices, std::vector<uint32_t> t_indices);
        explicit Mesh() = default;
        virtual ~Mesh();

        void load(const std::string&) {};

        inline void draw() const {
            glBindVertexArray(m_VAO);
            glDrawElements(GL_TRIANGLES, (GLsizei) m_indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        inline glm::vec3 getCenter() {
            return m_center;
        }

        glm::vec3 getNearest(glm::vec3);

    private:
        void setupMesh();
        void calcCenter();

        unsigned int m_VAO, m_VBO, m_EBO;

        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;
        glm::vec3 m_center;
    };
}


#endif //PE_RENDER_MESH_H
