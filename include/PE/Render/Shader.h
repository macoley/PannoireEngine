#ifndef PE_RENDER_SHADER_H
#define PE_RENDER_SHADER_H

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
     * Shader class
     */
    class Shader : public Resource::IResource {
        using ManagerPtr = std::shared_ptr<Resource::ResourceManager>;

    public:
        explicit Shader(ManagerPtr manager) : m_manager(std::move(manager)) {};
        Shader() = delete;
        virtual ~Shader();

        void load(const std::string &path) override;

        /*
         * SETTERS
         */

        void use() const {
            glUseProgram(m_shaderProgramID);
        }

        void set(const std::string &name, bool value) const {
            glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), (int) value);
        }

        void set(const std::string &name, int value) const {
            glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
        }

        void set(const std::string &name, float value) const {
            glUniform1f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
        }

        void set(const std::string &name, const glm::mat4 &matrix) const {
            glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        void set(const std::string &name, glm::vec3 v) const {
            glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), v.x, v.y, v.z);
        }

        void set(const std::string &name, float v0, float v1, float v2) const {
            glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), v0, v1, v2);
        }

    private:
        void compile(ProgramID vertexShaderID, ProgramID fragmentShaderID);

        ProgramID m_shaderProgramID{0};
        ManagerPtr m_manager;
    };

    /**
     * Vertex Shader
     */
    class VertexShader : public Resource::IResource {
        friend class Shader;
    public:
        VertexShader() = default;

        virtual ~VertexShader();

        void load(const std::string &path) override;

    private:
        void compile(const std::string &source);

        ProgramID m_vertexShaderID{0};
    };

    /**
     * Vertex Shader
     */
    class FragmentShader : public Resource::IResource {
        friend class Shader;

    public:
        FragmentShader() = default;
        virtual ~FragmentShader();

        void load(const std::string &path) override;

    private:
        void compile(const std::string &source);

        ProgramID m_fragmentShaderID{0};
    };

}

#endif //PE_RENDER_SHADER_H
