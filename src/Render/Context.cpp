
#include <PE/Render/Context.h>

#include "PE/Render/Context.h"

namespace PE::Render {

    /*
     * TEMP CALLBACKS
     */

    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {

    }

    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {

    }

    void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

    }

    /**
     * Constructor
     * @param title
     * @param width
     * @param height
     */
    Context::Context(const std::string &title, uint32_t width, uint32_t height)
            : m_width(width),
              m_height(height),
              m_resizeCallback([](uint32_t, uint32_t) {}),
              m_inputCallback([](uint32_t, uint32_t) {})
    {
        Utils::log("Context creating...");

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (m_window == NULL) {
            Utils::logError("Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        // Trick with pushin context to lambdas
        glfwSetWindowUserPointer(m_window, this);
        glfwMakeContextCurrent(m_window);

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
            auto context = static_cast<Context *>(glfwGetWindowUserPointer(window));

            context->m_resizeCallback(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
            glViewport(0, 0, width, height);
        });

        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            Utils::logError("Failed to initialize GLAD");
            return;
        }

        glfwSetScrollCallback(m_window, scroll_callback); // with gui
        glfwSetCursorPosCallback(m_window, mouse_callback);
        glfwSetMouseButtonCallback(m_window, mouse_button_callback); // with gui
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // or GLFW_CURSOR_DISABLED


        glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            auto context = static_cast<Context *>(glfwGetWindowUserPointer(window));

            context->m_inputCallback(static_cast<uint32_t>(key), static_cast<uint32_t>(action));
        });

        // Depth testing
        glEnable(GL_DEPTH_TEST);

        // Blending
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glDepthMask(GL_FALSE);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    Context::~Context() {
        Utils::log("Context destroying...");

        glfwTerminate();
    }

    bool Context::isRunning() {
        return !static_cast<bool>(glfwWindowShouldClose(m_window));
    }

    void Context::processInput() {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window, true);
    }

    double Context::getTime() {
        return glfwGetTime();
    }

    void Context::pollEvents() {
        glfwPollEvents();
    }

    void Context::render(Context::RenderFunction fnc, Color color) {
        glClearColor(color.r, color.g, color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fnc();

        glfwSwapBuffers(m_window);
    }
}
