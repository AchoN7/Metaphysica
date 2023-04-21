#pragma once

#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>

#include "GLdebug.hpp"
#include <glm/glm.hpp>

enum ShaderType {
    VERTEX = GL_VERTEX_SHADER,
    TESSCTRL = GL_TESS_CONTROL_SHADER,
    TESSEVAL = GL_TESS_EVALUATION_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    COMPUTE = GL_COMPUTE_SHADER
};

class Shader {
public:

    Shader() {
        m_programID = glCreateProgram();
        m_isBound = false;
    }

    void attachShader(ShaderType type, const char* path) {
        GLuint shader = glCreateShader(type);
        std::string shaderCode = readShaderSource(path);
        const char* shaderCodePtr = shaderCode.c_str();
        GL(glShaderSource(shader, 1, &shaderCodePtr, nullptr));
        GL(glCompileShader(shader));

        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status == GL_TRUE) {
            Logger::log(LogType::INFO, "Shader compilation successful.");
            GL(glAttachShader(m_programID, shader));
        }
        else {
            GLint logLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            char* log = new char[logLength + 1];
            glGetShaderInfoLog(shader, logLength, NULL, log);
            Logger::log(LogType::INFO, "Shader compilation fail. Log: \n", log);
            delete[] log;
        }  

        m_shaders.push_back(shader);
    }

    void link() { 
        GL(glLinkProgram(m_programID)); 

        GLint status;
        glGetProgramiv(m_programID, GL_LINK_STATUS, &status);

        if (status == GL_TRUE) {
            Logger::log(LogType::INFO, "Program link successful.");
        }
        else {
            GLint logLength;
            glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &logLength);
            char* log = new char[logLength + 1];
            glGetProgramInfoLog(m_programID, logLength, NULL, log);
            Logger::log(LogType::ERROR, "Program link fail. Log: ", log);
            delete[] log;
        }

        m_shaders.clear();
    }

    void bind() { GL(glUseProgram(m_programID)); m_isBound = true; }
    void unbind() { GL(glUseProgram(0)); m_isBound = false; }

    void setUniformFloat(const char* name, const float val) {
        if (!m_isBound) return;
        GLint location = glGetUniformLocation(m_programID, name);
        GL(glUniform1f(location, val));
    }

    void setUniformVec3(const char* name, const glm::vec3& vec) {
        if (!m_isBound) return;
        GLint location = glGetUniformLocation(m_programID, name);
        GL(glUniform3fv(location, 1, glm::value_ptr(vec)));
    }

    void setUniformVec4(const char* name, const glm::vec4& vec) {
        if (!m_isBound) return;
        GLint location = glGetUniformLocation(m_programID, name);
        GL(glUniform4fv(location, 1, glm::value_ptr(vec)));
    }

    void setUniformMat4(const char* name, const glm::mat4& mat) {
        if (!m_isBound) return;
        GLint location = glGetUniformLocation(m_programID, name);
        GL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void getUniformNames() {
        bind();

        GLint numUniforms;
        GL(glGetProgramiv(m_programID, GL_ACTIVE_UNIFORMS, &numUniforms));

        std::stringstream logString;
        logString << "Active uniforms in program: [" << numUniforms << "]\n";

        for (int i = 0; i < numUniforms; i++) {
            GLint nameLength, size;
            GLenum type;
            GLchar name[256];

            glGetActiveUniform(m_programID, i, sizeof(name) - 1, &nameLength, &size, &type, name);
            name[nameLength] = '\0';

            logString << '\t' << name << "\n";        
        }

        Logger::log(LogType::DEBUG, logString.str());

        unbind();
    }

    ~Shader() {
        GL(glDeleteProgram(m_programID));
    }

private:

    GLuint m_programID;
    bool m_isBound;
    std::vector<GLuint> m_shaders;

    static std::string readShaderSource(const char* shaderPath) {
        //currently assumes correct path to shader
        std::ifstream file(shaderPath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

};
