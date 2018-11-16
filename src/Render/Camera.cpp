
#include <PE/Render/Camera.h>

#include "PE/Render/Camera.h"

namespace PE::Render {

    Camera::Camera(uint32_t width, uint32_t height, float xPos, float yPos, float zPos, float pitch, float yaw)
            : cameraPos(xPos, yPos, zPos),
              cameraFront(0.0f, 0.0f, -1.0f),
              cameraUp(0.0f, 1.0f, 0.0f),
              pitch(pitch),
              yaw(yaw),
              fov(FOV),
              speed(SPEED),
              sensitivity(SENSIVITY)
    {
        calculateFront();
        calculateView();
        setProjection(width, height);
    }

    void Camera::move(Camera_Movement direction) {
        switch(direction)
        {
            case FORWARD:
                cameraPos += speed * cameraFront;
                break;
            case BACKWARD:
                cameraPos -= speed * cameraFront;
                break;
            case LEFT:
                cameraPos -= speed * glm::normalize(glm::cross(cameraFront, cameraUp));
                break;
            case RIGHT:
                cameraPos += speed * glm::normalize(glm::cross(cameraFront, cameraUp));
        }

        calculateView();
    }

    void Camera::rotate(float xoffset, float yoffset) {

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
            pitch =  89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        calculateFront();
        calculateView();
    }

    void Camera::calculateFront() {
        glm::vec3 front;
        front.x = (float) ( cos(glm::radians(yaw)) * cos(glm::radians(pitch)) );
        front.y = (float) sin(glm::radians(pitch));
        front.z = (float) ( cos(glm::radians(pitch)) * sin(glm::radians(yaw)) );

        cameraFront = glm::normalize(front);
    }

    void Camera::zoom(float yoffset) {

        if(fov >= 1.0f && fov <= 45.0f)
        {
            fov -= yoffset;
        }

        if(fov <= 1.0f)
        {
            fov = 1.0f;
        }
        else if(fov >= 45.0f)
        {
            fov = 45.0f;
        }
    }

    void Camera::calculateView() {
        m_view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void Camera::setProjection(uint32_t width, uint32_t height, float near, float far) {
        m_projection = glm::perspective(glm::radians(fov), (float) width / (float) height, near, far);
    }

}