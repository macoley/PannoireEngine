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

    struct Color {
        float r,g,b;
    };

    class Context {
        using RenderFunction = std::function<void ()>;
        using ResizeFunction = std::function<void (uint32_t, uint32_t)>;
        using InputFunction = std::function<void (uint32_t, uint32_t)>;

    public:
        Context(const std::string &, uint32_t, uint32_t);
        virtual ~Context();

        double getTime();

        bool isRunning();
        void processInput();
        void pollEvents();
        void render(RenderFunction, Color);

        void setResizeCallback(ResizeFunction fnc) {
            m_resizeCallback = std::move(fnc);
        }

        void setInputCallback(InputFunction fnc) {
            m_inputCallback = std::move(fnc);
        }

    private:
        GLFWwindow* m_window;

        glm::mat4 m_projection;
        uint32_t m_width, m_height;

        glm::mat4 m_temp_model;

        ResizeFunction m_resizeCallback;
        InputFunction m_inputCallback;
    };
}

#endif //PANNOIREENGINE_CONTEXT_H
