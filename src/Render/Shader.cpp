
#include "PE/Render/Shader.h"

namespace PE::Render {

    Shader::Shader(const std::string & pathVertex, const std::string & pathFragment) {
        compileShaders(loadSource(pathVertex), loadSource(pathFragment));
    }


    Shader::~Shader() {
        glDeleteProgram(shaderProgramID);
    }

    std::string Shader::loadSource(const std::string & path) {
        std::string shader;
        std::ifstream file;
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            std::stringstream stream;
            file.open(path);
            stream << file.rdbuf();
            file.close();

            shader = stream.str();
        }
        catch (const std::ifstream::failure & e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        return shader;
    }

    void Shader::compileShaders(const std::string &vertexSource, const std::string &fragmentSource) {
        // Temp vars
        uint32_t vertexShaderID, fragmentShaderID;

        int  success;
        char infoLog[512];

        /**
         * Vertex Shader
         */
        const char * vertexShaderSource = vertexSource.c_str();
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
        const char * fragmentShaderSource = fragmentSource.c_str();
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

}