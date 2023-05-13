#include "Shader.hpp"

Shader::Shader() {
    m_programID = glCreateProgram();
    m_isBound = false;
}

void Shader::attachShader(ShaderType type, const char* path) {
    GLuint shader = glCreateShader(type);
    std::string shaderCode = readShaderSource(path);
    const char* shaderCodePtr = shaderCode.c_str();
    GL(glShaderSource(shader, 1, &shaderCodePtr, nullptr));
    GL(glCompileShader(shader));

    COMPILE_STATUS(shader);
    GL(glAttachShader(m_programID, shader));
    m_shaders.push_back(shader);
}

void Shader::link() {
    GL(glLinkProgram(m_programID));
    LINK_STATUS(m_programID);
    m_shaders.clear();
}

void Shader::bind() {
    if (m_isBound) return;
    GL(glUseProgram(m_programID));
    m_isBound = true;
}

void Shader::unbind() {
    if (m_isBound) {
        GL(glUseProgram(0));
        m_isBound = false;
    }
}

void Shader::setUniformInt(const char* name, const int val) {
    if (!m_isBound) return;
    GLint location = getUniformLocation(name);
    GL(glUniform1i(location, val););
}

void Shader::setUniformFloat(const char* name, const float val) {
    if (!m_isBound) return;
    GLint location = getUniformLocation(name);
    GL(glUniform1f(location, val));
}

void Shader::setUniformVec3(const char* name, const glm::vec3& vec) {
    if (!m_isBound) return;
    GLint location = getUniformLocation(name);
    GL(glUniform3fv(location, 1, glm::value_ptr(vec)));
}

void Shader::setUniformVec4(const char* name, const glm::vec4& vec) {
    if (!m_isBound) return;
    GLint location = getUniformLocation(name);
    GL(glUniform4fv(location, 1, glm::value_ptr(vec)));
}

void Shader::setUniformMat4(const char* name, const glm::mat4& mat) {
    if (!m_isBound) return;
    GLint location = getUniformLocation(name);
    GL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
}

void Shader::getUniformNames() {
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

Shader::~Shader() {
    GL(glDeleteProgram(m_programID));
}

GLuint Shader::getUniformLocation(const std::string_view& uniformName) const {
    auto it = m_uniformLocationCache.find(uniformName.data());
    if (it != m_uniformLocationCache.end())
        return it->second;

    GLuint location = glGetUniformLocation(m_programID, uniformName.data());
    m_uniformLocationCache[uniformName.data()] = location;
    return location;
}

std::string Shader::readShaderSource(const char* shaderPath) {
    //currently assumes correct path to shader
    std::ifstream file(shaderPath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}