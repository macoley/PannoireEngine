#ifndef PANNOIREENGINE_CAMERA_H
#define PANNOIREENGINE_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PE::Render {

    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    const float PITCH       = 0.0f;
    const float YAW         = -90.0f;
    const float FOV         = 45.0f;
    const float SPEED       = 0.015f;
    const float SENSIVITY   = 0.15f;

    class Camera {
    public:
        Camera(uint32_t width, uint32_t height, float pitch = PITCH, float yaw = YAW);

        void zoom(float yoffset);
        void move(Camera_Movement direction);
        void rotate(float xoffset, float yoffset);

        void setPos(float xPos, float yPos, float zPos);
        glm::vec3 getPos() { return m_cameraPos; };

        float getFov() { return m_fov; };

        void setProjection(uint32_t width, uint32_t height, float near = .1f, float far = 100.0f);
        const glm::mat4 &getView() { return m_view; };
        const glm::mat4 &getProjection() { return m_projection; }

    private:
        void calculateFront();
        void calculateView();

        glm::vec3 m_cameraPos;
        glm::vec3 m_cameraFront;
        glm::vec3 m_cameraUp;

        float m_pitch, m_yaw;
        float m_fov;

        float m_speed;
        float m_sensitivity;

        glm::mat4 m_view;
        glm::mat4 m_projection;
    };


}

#endif //PANNOIREENGINE_CAMERA_H
