
#include "PE/Render/Shader.h"

namespace PE::Render {

    Shader::Shader(const std::string & vertexPath, const std::string & fragmentPath) {
        unsigned int vertexShaderID, fragmentShaderID;

        int  success;
        char infoLog[512];

        // Vertex Shader
        std::string vertexShader = openFile(vertexPath);
        const char * vertexShaderSource = vertexShader.c_str();
        vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShaderID);

        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Fragment shader
        std::string fragmentShader = openFile(fragmentPath);
        const char * fragmentShaderSource = fragmentShader.c_str();

        fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShaderID);

        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Shader program
        shaderProgramID = glCreateProgram();

        glAttachShader(shaderProgramID, vertexShaderID);
        glAttachShader(shaderProgramID, fragmentShaderID);
        glLinkProgram(shaderProgramID);

        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        }

        //

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    std::string Shader::openFile(const std::string & path) {
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

}