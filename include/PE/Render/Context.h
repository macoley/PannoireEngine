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

namespace PE::Render {

    class Context {
    public:
        Context();
        virtual ~Context();

        bool isRunning();
        void processInput();
        double getTime();
        void pollEvents();

        void swapBuffers();
        void clear();

    private:
        GLFWwindow* m_window;
    };
}

#endif //PANNOIREENGINE_CONTEXT_H
