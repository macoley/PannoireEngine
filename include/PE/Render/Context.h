#ifndef PANNOIREENGINE_CONTEXT_H
#define PANNOIREENGINE_CONTEXT_H

#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include <cassert>
#include <cmath>
#include <memory>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Shader.h"
#include "PE/Utils/Utils.h"

namespace PE::Render {

    class Context {
        using RenderFunction = std::function<void ()>;

    public:
        Context(const std::string &, uint32_t, uint32_t);
        virtual ~Context();

        double getTime();

        bool isRunning();
        void processInput();
        void pollEvents();
        void render(RenderFunction);

        void configCamera(Shader &shader, Camera *camera);

    private:
        GLFWwindow* m_window;

        glm::mat4 m_projection;
        uint32_t m_width, m_height;

        glm::mat4 m_temp_model;
    };
}

#endif //PANNOIREENGINE_CONTEXT_H
