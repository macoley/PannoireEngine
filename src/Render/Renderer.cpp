
#include <PE/Render/Renderer.h>

#include "PE/Render/Renderer.h"

namespace PE::Render {

    void Renderer::render(Shader &shader, double alpha) const {
        shader.use();

        shader.set("projection", m_camera->getProjection());
        shader.set("view", m_camera->getView());

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        //render(shader, m_opaque);

        //glDepthMask(GL_FALSE);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        render(shader, m_transparent);

        //glDisable(GL_BLEND);
        //glDepthMask(GL_TRUE);
    }

    void Renderer::sort() {
        std::sort(m_transparent.begin(), m_transparent.end(), [&](const RenderObject &a, const RenderObject &b) {
            return glm::length(m_camera->getPos() - (a.m_position)) <
                   glm::length(m_camera->getPos() - (b.m_position)); // todo optimize
        });
    }

    void Renderer::reset() {
        m_opaque.clear();
        m_transparent.clear();
    }

    void Renderer::add(Mesh *mesh, Material *material,
                       float xPos, float yPos, float zPos,
                       float xAngle, float yAngle, float zAngle,
                       float xScale, float yScale, float zScale) {

        RenderObject obj;
        obj.m_mesh = mesh;
        obj.m_material = material;
        obj.m_position = glm::vec3(xPos, yPos, zPos);
        obj.m_rotation = glm::vec3(xAngle, yAngle, zAngle);
        obj.m_scale = glm::vec3(xScale, yScale, zScale);

        if (obj.m_material->isTransparent())
            m_transparent.push_back(obj);
        else
            m_opaque.push_back(obj);

    }

}