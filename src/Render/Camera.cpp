
#include <PE/Render/Camera.h>

#include "PE/Render/Camera.h"

namespace PE::Render {

    Camera::Camera(uint32_t width, uint32_t height, float pitch, float yaw)
            : m_cameraPos(0.0f, 0.0f, 0.0f),
              m_cameraFront(0.0f, 0.0f, -1.0f),
              m_cameraUp(0.0f, 1.0f, 0.0f),
              m_pitch(pitch),
              m_yaw(yaw),
              m_fov(FOV),
              m_speed(SPEED),
              m_sensitivity(SENSIVITY) {
        calculateFront();
        calculateView();
        setProjection(width, height);
    }

    void Camera::move(Camera_Movement direction) {
        switch (direction) {
            case FORWARD:
                m_cameraPos += m_speed * m_cameraFront;
                break;
            case BACKWARD:
                m_cameraPos -= m_speed * m_cameraFront;
                break;
            case LEFT:
                m_cameraPos -= m_speed * glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
                break;
            case RIGHT:
                m_cameraPos += m_speed * glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
        }

        calculateView();
    }

    void Camera::rotate(float xoffset, float yoffset) {

        xoffset *= m_sensitivity;
        yoffset *= m_sensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;

        if (m_yaw > 89.0f)
            m_yaw = 89.0f;
        if (m_yaw < -89.0f)
            m_yaw = -89.0f;

        calculateFront();
        calculateView();
    }

    void Camera::calculateFront() {
        glm::vec3 front;
        front.x = (float) (cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
        front.y = (float) sin(glm::radians(m_pitch));
        front.z = (float) (cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw)));

        m_cameraFront = glm::normalize(front);
    }

    void Camera::zoom(float yoffset) {

        if (m_fov >= 1.0f && m_fov <= 45.0f) {
            m_fov -= yoffset;
        }

        if (m_fov <= 1.0f) {
            m_fov = 1.0f;
        } else if (m_fov >= 45.0f) {
            m_fov = 45.0f;
        }
    }

    void Camera::calculateView() {
        m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    }

    void Camera::setProjection(uint32_t width, uint32_t height, float near, float far) {
        m_projection = glm::perspective(glm::radians(m_fov), (float) width / (float) height, near, far);
    }

    void Camera::setPos(float xPos, float yPos, float zPos) {
        m_cameraPos = glm::vec3(xPos, yPos, zPos);
        calculateView();
    }

}