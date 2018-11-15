
#include <PE/Render/Context.h>

#include "PE/Render/Context.h"

namespace PE::Render {

    /*
     * TEMP CALLBACKS
     */

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {

    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {

    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {

    }

    void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    /**
     * Constructor
     * @param title
     * @param width
     * @param height
     */
    Context::Context(const std::string & title, uint32_t width, uint32_t height) : m_width(width), m_height(height)
    {
        Utils::log("Context creating...");

        m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (m_window == NULL)
        {
            Utils::logError("Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Utils::logError("Failed to initialize GLAD");
            return;
        }

        glfwSetScrollCallback(m_window, scroll_callback); // with gui
        glfwSetCursorPosCallback(m_window, mouse_callback);
        glfwSetMouseButtonCallback(m_window, mouse_button_callback); // with gui
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // or GLFW_CURSOR_DISABLED

        // Depth testing
        glEnable(GL_DEPTH_TEST);

        // Blending
        //glEnable(GL_BLEND);
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
        if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window, true);
    }

    double Context::getTime() {
        return glfwGetTime();
    }

    void Context::pollEvents() {
        glfwPollEvents();
    }

    void Context::render(Context::RenderFunction fnc) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fnc();

        glfwSwapBuffers(m_window);
    }

    void Context::configCamera(Shader &shader, Camera *camera) {
        shader.use();

        m_projection = glm::perspective(glm::radians(camera->getFov()), (float)m_width / (float)m_height, 0.1f, 100.0f);

        shader.set("projection", m_projection);
        shader.set("view", camera->getView());

        // render the loaded model
        glm::mat4 m_temp_model;
        m_temp_model = glm::translate(m_temp_model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        m_temp_model = glm::scale(m_temp_model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
        shader.set("model", m_temp_model);
    }
}
