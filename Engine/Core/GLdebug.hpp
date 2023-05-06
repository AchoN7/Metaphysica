#pragma once

#include <sstream>
#include <string>

#include <GL/glew.h>

#include "Core/Logger.hpp"

#ifdef _DEBUG

    #define GL(func) do { func; GLdebug::checkGLError(#func, __FILE__, __LINE__); } while (0)
    #define COMPILE_STATUS(shader) GLdebug::shaderCompileStatus(shader);
    #define LINK_STATUS(programID) GLdebug::programLinkStatus(programID);    

#else
    
    #define GL(func) func
    #define COMPILE_STATUS(shader)
    #define LINK_STATUS(programID)

#endif

class GLdebug {
public:

    static void shaderCompileStatus(GLuint shader);

    static void programLinkStatus(GLuint programID);

    static void checkGLError(const char* function, const char* file, int line);

    static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei length, const GLchar* message, const void* userParam);

    static void checkGLversion();
};