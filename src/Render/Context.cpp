#include "PE/Render/Context.h"

namespace PE::Render {

    Context::Context() {

        // GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_window = glfwCreateWindow(800, 600, "Engine", NULL, NULL);
        if (m_window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_window);

        // GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }

        glViewport(0, 0, 800, 600);
        glfwSetFramebufferSizeCallback(m_window, &Context::framebuffer_size_callback);

        // Depth testing
        glEnable(GL_DEPTH_TEST);


        //glfwSetScrollCallback(window, scroll_callback); // with gui
        //glfwSetCursorPosCallback(window, mouse_callback);
        //glfwSetMouseButtonCallback(window, mouse_button_callback); // with gui
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    }

    Context::~Context() {
        glfwTerminate();
    }

    void Context::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
        std::cout << "dziala";
    }

    bool Context::isRunning() {
        return !static_cast<bool>(glfwWindowShouldClose(m_window));
    }

    void Context::render() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render

        glfwSwapBuffers(m_window);
    }

    void Context::processInput() {
        if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window, true);

        /*
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            camera.move(FORWARD);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.move(BACKWARD);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.move(LEFT);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.move(RIGHT);
        */
    }

    double Context::getTime() {
        return glfwGetTime();
    }

    void Context::update() {
        glfwPollEvents();
    }
}
