#ifndef PE_RENDER_RENDERER_H
#define PE_RENDER_RENDERER_H

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "PE/Resource/Resource.h"

namespace PE::Render {

    class Mesh;
    class Material;
    class Shader;
    class Camera;
    class Model;

    /**
     * Renderer
     */
    class Renderer {
        struct RenderObject {
            Mesh *m_mesh;
            Material *m_material;

            glm::vec3 m_position;
            glm::vec3 m_rotation;
            glm::vec3 m_scale;
        };

        using ObjectContainer = std::vector<RenderObject>;
        using CameraPtr = std::shared_ptr<Camera>;

    public:
        explicit Renderer(CameraPtr camera) : m_camera(std::move(camera)) {};

        virtual ~Renderer() = default;

        void render(Shader &shader, double alpha) const;

        void add(Mesh *, Material *,
                 float xPos, float yPos, float zPos,
                 float xAngle, float yAngle, float zAngle,
                 float xScale, float yScale, float zScale);

        void sort();

        void reset();

    private:
        inline void render(Shader &shader, const ObjectContainer &objs) const;

        ObjectContainer m_opaque;
        ObjectContainer m_transparent;
        CameraPtr m_camera;
    };

    void Renderer::render(Shader &shader, const Renderer::ObjectContainer &objs) const {
        static glm::mat4 matrix;

        for (const RenderObject &obj : objs) {
            matrix = glm::mat4();
            //matrix = glm::rotate(matrix, 360.0f, obj.m_rotation);
            matrix = glm::translate(matrix, obj.m_position);
            matrix = glm::scale(matrix, obj.m_scale);
            shader.set("model", matrix);

            obj.m_material->bind(shader);
            obj.m_mesh->draw();
        }
    }

}

#endif //PE_RENDER_RENDERER_H
