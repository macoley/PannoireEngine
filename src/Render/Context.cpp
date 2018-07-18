#include "PE/Render/Context.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace PE::Render {

    // settings
    const unsigned int SCR_WIDTH = 1200;
    const unsigned int SCR_HEIGHT = 1000;

    float cursorX = 400, cursorY = 300;
    bool movingCamera = false;
    Camera camera(glm::vec3(0.5f, 0.5f, 4.0f));

    void processInput2(GLFWwindow *window)
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.move(FORWARD);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.move(BACKWARD);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.move(LEFT);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.move(RIGHT);
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            movingCamera = true;

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            cursorX = (float) (xpos);
            cursorY = (float) (ypos);
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            movingCamera = false;
        }
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.zoom(yoffset);
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if(movingCamera)
        {
            float xoffset = (float) (xpos - cursorX);
            float yoffset = (float) (cursorY - ypos);

            camera.rotate(xoffset, yoffset);

            cursorX = (float) xpos;
            cursorY = (float) ypos;
        }
    }


    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    Context::Context() {

        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // glfw window creation
        // --------------------
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PannoireEngine", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }

        glfwSetScrollCallback(window, scroll_callback); // with gui
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback); // with gui
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Shader shader("1.model_loading.vs", "1.model_loading.fs");
        //Model model("nanosuit/nanosuit.obj");
        Model model("island/ForestScene.obj");

        Shader lightingShader("shaders/lightVS.glsl", "shaders/lightFS.glsl");

        /*
         * debug light
         */

        float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        unsigned int lightVAO, lightVBO;

        glGenBuffers(1, &lightVBO);
        glGenVertexArrays(1, &lightVAO);

        glBindVertexArray(lightVAO);

        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);


        //

        shader.use();
        shader.setVec3("lightColor",  1.0f, 0.905f, 0.196f);

        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // input
            // -----
            processInput2(window);

            // render
            // ------
            glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //
            shader.use();

            lightPos = glm::vec3((float)(sin(glfwGetTime()/3.0f) * 2.0f), 0.0, (float)(cos(glfwGetTime()/3.0f) * 2.0f));
            shader.setVec3("lightPos", lightPos);

            glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.getView();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);

            // render the loaded model
            glm::mat4 modelmx;
            modelmx = glm::translate(modelmx, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
            modelmx = glm::scale(modelmx, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
            shader.setMat4("model", modelmx);
            model.draw(shader);

            //

            lightingShader.use();
            glm::mat4 lightModel = glm::mat4();
            lightModel = glm::translate(lightModel, lightPos);
            lightModel = glm::scale(lightModel, glm::vec3(0.2f));

            lightingShader.setMat4("model", lightModel);
            lightingShader.setMat4("view", view);
            lightingShader.setMat4("projection", projection);
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }


        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
    }

    Context::~Context() {
        glfwTerminate();
    }

    void Context::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    bool Context::isRunning() {
        return !static_cast<bool>(glfwWindowShouldClose(m_window));
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

    void Context::swapBuffers() {
        glfwSwapBuffers(m_window);
    }

    void Context::clear() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
