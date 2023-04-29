#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLdebug.hpp"

class Uniform {
public:
    Uniform(const std::string& name, GLuint programID)
        : m_name(name)
    {
        m_location = GL(glGetUniformLocation(programID, m_name.c_str()));
    }

    void set(float value) const {
        glUniform1f(m_location, value);
    }

    void set(glm::vec3 value) const {
        glUniform3fv(m_location, 1, glm::value_ptr(value));
    }

    void set(const glm::mat4& matrix) const {
        glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

private:
    std::string m_name;
    GLint m_location;
};