#pragma once

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "Core/Logger.hpp"

class Renderer {
public:

	Renderer(int vpWidth, int vpHeight) {
		glewInit();

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debugCallback, nullptr);

        float vertices[] = { 0.5f, 0.5f, 0.0f,  // top right
                            0.5f, -0.5f, 0.0f,  // bottom right
                            -0.5f, -0.5f, 0.0f,  // bottom left
                            -0.5f, 0.5f, 0.0f }; // top left
        unsigned int indices[] = { 0, 1, 3,  // first triangle
                                1, 2, 3 }; // second triangle

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Compile and link shaders
        const char* vertexShaderPath = "Shaders/Vertex.shader";
        const char* fragmentShaderPath = "Shaders/Fragment.shader";

        // Vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        std::string vxShaderCode = readShaderSource(vertexShaderPath);
        const char* vxShaderCodePtr = vxShaderCode.c_str();
        glShaderSource(vertexShader, 1, &vxShaderCodePtr, nullptr);
        glCompileShader(vertexShader);

        // Fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        std::string frShaderCode = readShaderSource(fragmentShaderPath);
        const char* frShaderCodePtr = frShaderCode.c_str();
        glShaderSource(fragmentShader, 1, &frShaderCodePtr, nullptr);
        glCompileShader(fragmentShader);

        // Shader program
        m_program = glCreateProgram();
        glAttachShader(m_program, vertexShader);
        glAttachShader(m_program, fragmentShader);
        glLinkProgram(m_program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);


        //Framebuffer and Image
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
            Logger::log(LogType::ERROR, "Framebuffer is not complete! Check Renderer constructor!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        Logger::log(LogType::INFO, "Renderer initialized successfully!");
	}

    void recreateFramebuffer(int vpWidth, int vpHeight) {
        glDeleteTextures(1, &m_image);
        glDeleteRenderbuffers(1, &m_RBO);
        glDeleteFramebuffers(1, &m_FBO);

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

    }

    void render() {
        glUseProgram(m_program);
        glBindVertexArray(m_VAO);
        
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindVertexArray(0);

    }

    GLuint getImage() const { return m_image; }

    void shutdown() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        glDeleteProgram(m_program);
    }

private:

	GLuint m_program;
	GLuint m_image;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_FBO;
	GLuint m_RBO;
    GLuint m_EBO;
	
    std::string readShaderSource(const char* shaderPath) {
        std::ifstream file(shaderPath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei length, const GLchar* message, const void* userParam) {
        Logger::log(LogType::OPENGL, message);
    }

};