
#include "PE/Render/Shader.h"

namespace PE::Render {

    Shader::Shader(const std::string & vertexShader, const std::string & fragmentShader) {
        // Temp vars
        uint32_t vertexShaderID, fragmentShaderID;

        int  success;
        char infoLog[512];

        /**
         * Vertex Shader
         */
        const char * vertexShaderSource = vertexShader.c_str();
        vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShaderID);

        // get results
        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; // Todo logger
        }


        /**
         * Fragment Shader
         */
        const char * fragmentShaderSource = fragmentShader.c_str();
        fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShaderID);

        // get results
        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl; // Todo logger
        }

        /**
         * Compile entire Shader Program
         */
        shaderProgramID = glCreateProgram();

        glAttachShader(shaderProgramID, vertexShaderID);
        glAttachShader(shaderProgramID, fragmentShaderID);
        glLinkProgram(shaderProgramID);

        // get results
        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl; // Todo logger
        }


        // Clear Vertex and fragment shader
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }


    Shader::~Shader() {
        glDeleteProgram(shaderProgramID);
    }


    // Move to resource manager
    /*
    std::string Shader::openFile(const std::string & path) {

    }
     */
}