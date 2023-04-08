#pragma once

#include <sstream>
#include <string>

#include <GL/glew.h>

#include "Core/Logger.hpp"

#ifdef _DEBUG
#define GL(func) do { func; GLdebug::checkGLError(#func, __FILE__, __LINE__); } while (0)
#else
#define GL(func) func
#endif

class GLdebug {
public:

    static void checkGLError(const char* function, const char* file, int line) {
        GLenum error = glGetError();
        while (error != GL_NO_ERROR) {
            const GLubyte* errorString = glewGetErrorString(error);
            std::stringstream errorLog;
            errorLog << "[Error in function: " << function << "]\n";
            errorLog << "\tFile: " << file << ", line: " << line << "\n";
            errorLog << "\tCode: " << error << " - " << errorString;
            Logger::log(LogType::OPENGL, errorLog.str());
            error = glGetError();
        }
    }

    static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei length, const GLchar* message, const void* userParam) {
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

        Logger::log(LogType::OPENGL, logString.str());
    }

    static void checkGLversion() {
        std::stringstream versionLog;
        versionLog << "OpenGL version: [";
        versionLog << glGetString(GL_VERSION) << "].";
        Logger::log(LogType::OPENGL, versionLog.str());
    }
};