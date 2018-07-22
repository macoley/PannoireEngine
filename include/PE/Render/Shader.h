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

namespace PE::Render {

    class Shader {
        using programID = uint32_t;

    public:
        Shader(const std::string & vertexPath, const std::string & fragmentPath);
        virtual ~Shader();

        inline void use() const {
            glUseProgram(shaderProgramID);
        }

        inline programID getProgramID() const {
            return shaderProgramID;
        }

        /*
         * SETTERS
         */

        inline void setBool(const std::string &name, bool value) const {
            glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int) value);
        }

        inline void setInt(const std::string &name, int value) const {
            glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
        }

        inline void setFloat(const std::string &name, float value) const {
            glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
        }

        inline void setMat4(const std::string &name, const glm::mat4& matrix) const {
            glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        inline void setVec3(const std::string &name, float v0, float v1, float v2) const {
            glUniform3f(glGetUniformLocation(shaderProgramID, name.c_str()), v0, v1, v2);
        }

        inline void setVec3(const std::string &name, glm::vec3 v) const {
            glUniform3f(glGetUniformLocation(shaderProgramID, name.c_str()), v.x, v.y, v.z);
        }

    private:
        programID shaderProgramID;

        //std::string openFile(const std::string & path);
    };

}

#endif //PANNOIREENGINE_SHADER_H
