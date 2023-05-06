#include "GLdebug.hpp"

void GLdebug::shaderCompileStatus(GLuint shader) {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_TRUE) {
        Logger::log(LogType::INFO, "Shader compilation successful.");
    }
    else {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(logLength, '\0');
        glGetShaderInfoLog(shader, logLength, NULL, &log[0]);
        Logger::log(LogType::ERROR, "Shader compilation fail. Reason: \n", log);
    }
}

void GLdebug::programLinkStatus(GLuint programID) {
    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);

    if (status == GL_TRUE) {
        Logger::log(LogType::INFO, "Program link successful.");
    }
    else {
        GLint logLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(logLength, '\0');
        glGetProgramInfoLog(programID, logLength, NULL, &log[0]);
        Logger::log(LogType::ERROR, "Program link fail. Reason: \n", log);
    }
}

void GLdebug::checkGLError(const char* function, const char* file, int line) {
    GLenum error = glGetError();
    bool hasError = false;
    while (error != GL_NO_ERROR) {
        hasError = true;
        const GLubyte* errorString = glewGetErrorString(error);

        std::stringstream errorLog;
        errorLog << "[Error in function: " << function << "]\n";
        errorLog << "\tFile: " << file << ", line: " << line << "\n";
        errorLog << "\tCode: " << error << " - " << errorString;

        Logger::log(LogType::OPENGL, errorLog.str());
        error = glGetError();
    }
    glGetError(); //to not repeat the same log

#ifdef _DEBUG
    if (hasError) {
        __debugbreak();
    }
#endif
}

void GLdebug::debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam) {

    static std::string lastLogMessage = "";

    std::stringstream logString;
    logString << message << " [ID: " << id << "]\n";

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        logString << "\tSource: API\n";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        logString << "\tSource: Window system\n";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        logString << "\tSource: Shader compiler\n";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        logString << "\tSource: Third party\n";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        logString << "\tSource: Application\n";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        logString << "\tSource: Other\n";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        logString << "\tType: Error\n";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        logString << "\tType: Deprecated behavior\n";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        logString << "\tType: Undefined behavior\n";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        logString << "\tType: Portability\n";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        logString << "\tType: Performance\n";
        break;
    case GL_DEBUG_TYPE_MARKER:
        logString << "\tType: Marker\n";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        logString << "\tType: Push group\n";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        logString << "\tType: Pop group\n";
        break;
    case GL_DEBUG_TYPE_OTHER:
        logString << "\tType: Other\n";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        logString << "\tSeverity: High\n";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        logString << "\tSeverity: Medium\n";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        logString << "\tSeverity: Low\n";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        logString << "\tSeverity: Notification\n";
        break;
    }

    std::string newLogMessage = logString.str();
    if (newLogMessage != lastLogMessage) {
        Logger::log(LogType::OPENGL, newLogMessage);
        lastLogMessage = newLogMessage;
    }
}

void GLdebug::checkGLversion() {
    std::stringstream versionLog;
    versionLog << "OpenGL version: [";
    versionLog << glGetString(GL_VERSION) << "].";
    Logger::log(LogType::OPENGL, versionLog.str());
}