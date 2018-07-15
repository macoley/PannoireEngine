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

namespace PE::Render {

    class Context : public PE::Engine::IContext {
    public:
        Context();
        virtual ~Context();

        bool isRunning() override;
        void render() override;
        void processInput() override;
        virtual double getTime() override;
        virtual void update() override;

    private:
        GLFWwindow* m_window;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    };
}

#endif //PANNOIREENGINE_CONTEXT_H
