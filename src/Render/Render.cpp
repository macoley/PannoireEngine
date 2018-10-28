
#include "PE/Render/Render.h"

namespace PE::Render {

    void init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    Context* createContext(const std::string& title, uint32_t width, uint32_t height) {
        return new Context(title, width, height);
    }

}