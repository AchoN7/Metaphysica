#pragma once

#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Core/GLdebug.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    Shader();

    void attachShader(ShaderType type, const char* path);
    void link();
    void bind();
    void unbind();

    void setUniformInt(const char* name, const int val);
    void setUniformFloat(const char* name, const float val);
    void setUniformVec3(const char* name, const glm::vec3& vec);
    void setUniformVec4(const char* name, const glm::vec4& vec);
    void setUniformMat4(const char* name, const glm::mat4& mat);

    void getUniformNames();
    ~Shader();

private:

    GLuint m_programID;
    bool m_isBound;
    std::vector<GLuint> m_shaders;
    mutable std::unordered_map<std::string, GLuint> m_uniformLocationCache;

    GLuint getUniformLocation(const std::string_view& uniformName) const;
    static std::string readShaderSource(const char* shaderPath);

};
