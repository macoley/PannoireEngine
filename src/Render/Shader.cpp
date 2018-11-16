
#include <PE/Render/Shader.h>

#include "PE/Render/Shader.h"

namespace PE::Render {

    /**
     * Helper function
     * @param path
     * @return
     */
    std::string loadSource(const std::string &path) {
        std::string source;
        std::ifstream file;
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            std::stringstream stream;
            file.open(path);
            stream << file.rdbuf();
            file.close();

            source = stream.str();
        }
        catch (const std::ifstream::failure & e)
        {
            Utils::logError("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
        }

        return source;
    }


    /**
     * Shader Compile
     * @param vertexShaderID
     * @param fragmentShaderID
     */
    void Shader::compile(ProgramID vertexShaderID, ProgramID fragmentShaderID) {
        int  success;
        char infoLog[512];

        /**
         * Compile entire Shader Program
         */
        m_shaderProgramID = glCreateProgram();

        glAttachShader(m_shaderProgramID, vertexShaderID);
        glAttachShader(m_shaderProgramID, fragmentShaderID);
        glLinkProgram(m_shaderProgramID);

        // get results
        glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &success);

        if(!success) {
            glGetProgramInfoLog(m_shaderProgramID, 512, NULL, infoLog);

            Utils::logError("ERROR::SHADER::PROGRAM::LINK_FAILED " + std::string(infoLog));
        }
    }

    Shader::~Shader() {
        glDeleteProgram(m_shaderProgramID);
    }

    void Shader::load(const std::string &path) {
        auto properties = m_manager->load<Resource::Properties>(path);

        auto vertex = m_manager->load<VertexShader>(properties->get<std::string>("vertex_shader"));
        auto fragment = m_manager->load<FragmentShader>(properties->get<std::string>("fragment_shader"));

        compile(vertex->getID(), fragment->getID());
    }

    void Shader::use() const {
        glUseProgram(m_shaderProgramID);
    }

    void Shader::set(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), (int) value);
    }

    void Shader::set(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
    }

    void Shader::set(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
    }

    void Shader::set(const std::string &name, const glm::mat4 &matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::set(const std::string &name, glm::vec3 v) const {
        glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), v.x, v.y, v.z);
    }

    void Shader::set(const std::string &name, float v0, float v1, float v2) const {
        glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), v0, v1, v2);
    }

    /**
     * Vertex Shader load
     * @param path
     */
    void VertexShader::load(const std::string &path) {
        compile(loadSource(path));
    }

    void VertexShader::compile(const std::string &source) {
        int  success;
        char infoLog[512];

        /**
         * Vertex Shader
         */
        const char * vertexShaderSource = source.c_str();
        m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(m_vertexShaderID, 1, &vertexShaderSource, NULL);
        glCompileShader(m_vertexShaderID);

        // get results
        glGetShaderiv(m_vertexShaderID, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(m_vertexShaderID, 512, NULL, infoLog);

            Utils::logError("ERROR::SHADER::VERTEX::COMPILATION_FAILED " + std::string(infoLog));
        }
    }

    VertexShader::~VertexShader() {
        glDeleteShader(m_vertexShaderID);
    }

    /**
     * Fragment Shader load
     * @param path
     */
    void FragmentShader::load(const std::string &path) {
        compile(loadSource(path));
    }

    void FragmentShader::compile(const std::string &source) {
        int  success;
        char infoLog[512];

        const char * fragmentShaderSource = source.c_str();
        m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(m_fragmentShaderID, 1, &fragmentShaderSource, NULL);
        glCompileShader(m_fragmentShaderID);

        // get results
        glGetShaderiv(m_fragmentShaderID, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(m_fragmentShaderID, 512, NULL, infoLog);

            Utils::logError("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED " + std::string(infoLog));
        }
    }

    FragmentShader::~FragmentShader() {
        glDeleteShader(m_fragmentShaderID);
    }
}