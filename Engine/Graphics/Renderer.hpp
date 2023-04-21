#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLdebug.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Core/Logger.hpp"

#include "World/World.hpp"

class Renderer {
public:

	Renderer() {

#ifdef _DEBUG
        GLdebug::checkGLversion();
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(GLdebug::debugCallback, nullptr);
#endif
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
        glViewport(0, 0, m_vpWidth, m_vpHeight);
        glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
    } 

    void renderWorld(World& world) {
        GL(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 view = world.getCamera().getViewMatrix();
        glm::mat4 projection = world.getCamera().getProjectionMatrix();
        //glm::vec3 lightPos = world.getStarLight().position;
        glm::vec3 lightDir = world.getStarLight().direction;
        glm::vec3 lightColor = world.getStarLight().color;
        glm::vec3 camPos = world.getCamera().getPosition();

        m_starProgram.bind();
        m_starProgram.setUniformMat4("u_view", view);
        m_starProgram.setUniformMat4("u_projection", projection);
        renderStar(world.getStar());
        m_starProgram.unbind();

        m_modelProgram.bind();
        m_modelProgram.setUniformMat4("u_view", view);
        m_modelProgram.setUniformMat4("u_projection", projection);
        //m_modelProgram.setUniformVec3("u_lightPosition", lightPos);
        m_modelProgram.setUniformVec3("u_lightDirection", lightDir);
        m_modelProgram.setUniformVec3("u_lightColor", lightColor);
        m_modelProgram.setUniformVec3("u_cameraPosition", camPos);

        renderModel(world.getGround());

        for (auto& model : world.getModels()) {
            renderModel(model);
        }
       
        m_modelProgram.unbind();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Shader& getStarProgram() { return m_starProgram; }
    Shader& getModelProgram() { return m_modelProgram; }
    GLuint getImage() const { return m_image; }

    void shutdown() {
        if (glIsTexture(m_image)) glDeleteTextures(1, &m_image);
        if (glIsRenderbuffer(m_RBO)) glDeleteRenderbuffers(1, &m_RBO);
        if (glIsFramebuffer(m_FBO)) glDeleteFramebuffers(1, &m_FBO);
    }

#pragma region ** STATICS **

    static void bindModel(Model& model) {
        auto& mesh = *model.getMesh();

        glGenVertexArrays(1, &mesh.VAO);
        glBindVertexArray(mesh.VAO);

        glGenBuffers(1, &mesh.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        GL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW));

        glGenBuffers(1, &mesh.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), mesh.indices.data(), GL_STATIC_DRAW));

        // Position attribute
        glEnableVertexAttribArray(0);
        GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)));

        // Normal attribute
        glEnableVertexAttribArray(1);
        GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

        glBindVertexArray(0);
    }

    static void unbindObject(Model& model) {
        auto& mesh = *model.getMesh();

        if (mesh.VAO != 0) {
            glBindVertexArray(mesh.VAO);

            if (mesh.VBO != 0) {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDeleteBuffers(1, &mesh.VBO);
                mesh.VBO = 0;
            }

            if (mesh.EBO != 0) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                glDeleteBuffers(1, &mesh.EBO);
                mesh.EBO = 0;
            }

            glBindVertexArray(0);
            glDeleteVertexArrays(1, &mesh.VAO);
            mesh.VAO = 0;
        }

    }

#pragma endregion

#pragma region ** OPENGL SWITCHES **

    void enableDepthMask() { glDepthMask(GL_TRUE); }
    void enableBackfaceCull() { glEnable(GL_CULL_FACE); }

    void disableDepthMask() { glDepthMask(GL_FALSE); }
    void disableBackfaceCull() { glDisable(GL_CULL_FACE); }

#pragma endregion

private:

    Shader m_modelProgram;
    Shader m_starProgram;

	GLuint m_image = 0;
	GLuint m_FBO = 0;
    GLuint m_RBO = 0;
	
    int m_vpWidth = 0;
    int m_vpHeight = 0;

    void renderStar(Model& star) {
        auto& mesh = *star.getMesh();
        auto& material = star.getMaterial();

        m_starProgram.setUniformMat4("u_model", star.getModelMatrix());
        m_starProgram.setUniformVec3("u_starColor", material.color);

        
        disableDepthMask();
        GL(glBindVertexArray(mesh.VAO));

        GL(glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0));

        glBindVertexArray(0);
        enableDepthMask();
    }

    void renderModel(Model& model) {
        auto& mesh = *model.getMesh();
        auto& material = model.getMaterial();

        m_modelProgram.setUniformMat4("u_model", model.getModelMatrix());
        m_modelProgram.setUniformVec3("u_modelColor", material.color);

        GL(glBindVertexArray(mesh.VAO));

        GL(glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0));

        glBindVertexArray(0);
    }
	
};

