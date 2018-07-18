#ifndef PANNOIREENGINE_CONTEXT_H
#define PANNOIREENGINE_CONTEXT_H

#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include <cassert>
#include <cmath>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "PE/Engine/IContext.h"
#include "PE/Render/Shader.h"
#include "PE/Render/Mesh.h"
#include "PE/Render/Model.h"
#include "PE/Render/Camera.h"

namespace PE::Render {

    class Context : public PE::Engine::IContext {
    public:
        Context();
        virtual ~Context();

        bool isRunning() override;
        void processInput() override;
        double getTime() override;
        void update() override;

        void swapBuffers() override;
        void clear() override;

    private:
        GLFWwindow* m_window;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    };
}

#endif //PANNOIREENGINE_CONTEXT_H
