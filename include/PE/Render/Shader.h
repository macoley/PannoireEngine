#ifndef PANNOIREENGINE_SHADER_H
#define PANNOIREENGINE_SHADER_H

#include <fstream>
#include <string>
#include <streambuf>
#include <cassert>
#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PE/Resource/Resource.h"
#include "PE/Resource/Properties.h"

namespace PE::Render {

    using ProgramID = uint32_t;

    std::string loadSource(const std::string &path);

    /**
     * Vertex Shader
     */
    class VertexShader : public Resource::IResource {
    public:
        VertexShader() = default;

        virtual ~VertexShader();

        void load(const std::string &path) override;

        ProgramID getID() {
            return m_vertexShaderID;
        }

    private:
        void compile(const std::string &source);

        ProgramID m_vertexShaderID{0};
    };

    /**
     * Vertex Shader
     */
    class FragmentShader : public Resource::IResource {
    public:
        FragmentShader() = default;

        virtual ~FragmentShader();

        void load(const std::string &path) override;

        ProgramID getID() {
            return m_fragmentShaderID;
        }

    private:
        void compile(const std::string &source);

        ProgramID m_fragmentShaderID{0};
    };


    /**
     * Shader class
     */
    class Shader : public Resource::IResource {
        using ManagerPtr = std::shared_ptr<Resource::ResourceManager>;

    public:
        explicit Shader(ManagerPtr manager) : m_manager(std::move(manager)) {};
        Shader() = delete;
        virtual ~Shader();

        void load(const std::string &path) override;

        inline void use() const {
            glUseProgram(m_shaderProgramID);
        }

        ProgramID getID() {
            return m_shaderProgramID;
        }

        /*
         * SETTERS
         */
        template<typename T>
        void set(const std::string &name, T value)
        {}

        void set(const std::string &name, bool value)
        {
            glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), (int) value);
        }

        void set(const std::string &name, int value)
        {
            glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
        }

        void set(const std::string &name, float value)
        {
            glUniform1f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
        }

        void set(const std::string &name, const glm::mat4 &matrix)
        {
            glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        void set(const std::string &name, glm::vec3 v)
        {
            glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), v.x, v.y, v.z);
        }

        void set(const std::string &name, float v0, float v1, float v2)
        {
            glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), v0, v1, v2);
        }

    private:
        void compile(ProgramID vertexShaderID, ProgramID fragmentShaderID);

        ProgramID m_shaderProgramID{0};
        ManagerPtr m_manager;
    };

}

#endif //PANNOIREENGINE_SHADER_H
