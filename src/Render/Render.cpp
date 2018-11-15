
#include "PE/Render/Render.h"

namespace PE::Render {

    void init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    std::shared_ptr<Context> createContext(const std::string& title, uint32_t width, uint32_t height) {
        return std::make_shared<Context>(title, width, height);
    }

}