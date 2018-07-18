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
        Camera(glm::vec3 cameraPos = {0.0f, 0.0f, 0.0f}, float pitch = PITCH, float yaw = YAW);

        void zoom(float yoffset);
        void move(Camera_Movement direction);
        void rotate(float xoffset, float yoffset);

        float getFov() { return fov; };
        glm::mat4 getView() { return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); };
        glm::vec3 getPos() { return cameraPos; };

    private:
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;

        float pitch, yaw;
        float fov;

        float speed;
        float sensitivity;

        void calculateFront();
    };


}

#endif //PANNOIREENGINE_CAMERA_H
