#pragma once

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLdebug.hpp"
#include "Core/Logger.hpp"

#include "Objects/Sphere.hpp"


class Renderer {
public:

	Renderer() {
		glewInit();

#ifdef _DEBUG
        GLdebug::checkGLversion();
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(GLdebug::debugCallback, nullptr);
#endif
        glGenVertexArrays(1, &m_VAO);

        //compile shaders and link shader program
        const char* vertexShaderPath = "Shaders/Vertex.shader";
        const char* fragmentShaderPath = "Shaders/Fragment.shader";

        // Vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        std::string vxShaderCode = readShaderSource(vertexShaderPath);
        const char* vxShaderCodePtr = vxShaderCode.c_str();
        GL(glShaderSource(vertexShader, 1, &vxShaderCodePtr, nullptr));
        GL(glCompileShader(vertexShader));

        // Fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        std::string frShaderCode = readShaderSource(fragmentShaderPath);
        const char* frShaderCodePtr = frShaderCode.c_str();
        GL(glShaderSource(fragmentShader, 1, &frShaderCodePtr, nullptr));
        GL(glCompileShader(fragmentShader));

        // Shader program
        m_program = glCreateProgram();
        GL(glAttachShader(m_program, vertexShader));
        GL(glAttachShader(m_program, fragmentShader));
        GL(glLinkProgram(m_program));

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        Logger::log(LogType::INFO, "Renderer initialized successfully!");
	}

    void recreateFramebuffer(int vpWidth, int vpHeight) {
        if (glIsTexture(m_image)) glDeleteTextures(1, &m_image);
        if (glIsRenderbuffer(m_RBO)) glDeleteRenderbuffers(1, &m_RBO);
        if (glIsFramebuffer(m_FBO)) glDeleteFramebuffers(1, &m_FBO);

        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        glGenTextures(1, &m_image);
        glBindTexture(GL_TEXTURE_2D, m_image);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, vpWidth, vpHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_image, 0);

        glGenRenderbuffers(1, &m_RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, vpWidth, vpHeight);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Logger::log(LogType::ERROR, "Framebuffer is not complete! Check the recreateFramebuffer function in the Renderer!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_vpWidth = vpWidth;
        m_vpHeight = vpHeight;

    }

    void bindSphere(Sphere& sphere) {
        GL(glBindVertexArray(m_VAO));

        glGenBuffers(1, &sphere.VBO);
        glGenBuffers(1, &sphere.EBO);

        glBindBuffer(GL_ARRAY_BUFFER, sphere.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sphere.vertices.size(), sphere.vertices.data(), GL_STATIC_DRAW);

        GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.EBO));
        GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sphere.indecies.size(), sphere.indecies.data(), GL_STATIC_DRAW));

        GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0));
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    void detachSphere(Sphere& sphere) {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        GL(glDeleteBuffers(1, &sphere.VBO));
        GL(glDeleteBuffers(1, &sphere.EBO));
        sphere.vertices.clear();
        sphere.indecies.clear();
    }

    void renderSphere(const Sphere& sphere) {

        GL(glUseProgram(m_program));
        GL(glBindVertexArray(m_VAO));

        GL(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GL(glDrawElements(GL_TRIANGLES, sphere.indecies.size(), GL_UNSIGNED_INT, 0));

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindVertexArray(0);
    }

    void render() {
        GL(glUseProgram(m_program));
        GL(glBindVertexArray(m_VAO));
        
        GL(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindVertexArray(0);
    }

    void setVec4(const char* name, const glm::vec4& vec) const {
        GL(glUseProgram(m_program));

        GLint location = glGetUniformLocation(m_program, name);
        GL(glUniform4fv(location, 1, glm::value_ptr(vec)));
        
    }

    void setMat4(const char* name, const glm::mat4& mat) const {
        GL(glUseProgram(m_program));

        GLint location = glGetUniformLocation(m_program, name);
        GL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void wireframeMode(bool shouldTurnOn) {
        if (shouldTurnOn)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else 
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    GLuint getImage() const { return m_image; }

    void shutdown() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteProgram(m_program);
    }

private:

	GLuint m_program;

	GLuint m_image;
	GLuint m_VAO;
	//GLuint m_VBO;
	GLuint m_FBO;
	GLuint m_RBO;
    //GLuint m_EBO;

    int m_vpWidth;
    int m_vpHeight;
	
    static std::string readShaderSource(const char* shaderPath) {
        //currently assumes correct path to shader
        std::ifstream file(shaderPath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

};

