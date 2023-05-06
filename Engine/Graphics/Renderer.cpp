#include "Renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Core/GLdebug.hpp"
#include "Core/Logger.hpp"
#include "Scene/Camera.hpp"


Renderer::Renderer() : 
 m_image(0),
 m_FBO(0),
 m_RBO(0),
 m_shadowMapProgram(),
 m_depthFBO(0),
 m_depthMap(0),
 m_depthMapWidth(2 * 1024),
 m_depthMapHeight(2 * 1024),
 m_vpWidth(0),
 m_vpHeight(0) {

#ifdef _DEBUG
    GLdebug::checkGLversion();
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLdebug::debugCallback, nullptr);
#endif

    //create resources for shadow mapping
    glGenFramebuffers(1, &m_depthFBO);
    glGenTextures(1, &m_depthMap);

    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_depthMapWidth, m_depthMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glm::vec4 borderColor(1.0f);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));

    glBindFramebuffer(GL_FRAMEBUFFER, m_depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_shadowMapProgram.attachShader(ShaderType::VERTEX, "Shaders/Shadow.vert");
    m_shadowMapProgram.attachShader(ShaderType::FRAGMENT, "Shaders/Shadow.frag");
    m_shadowMapProgram.link();
}

void Renderer::recreateFramebuffer(int vpWidth, int vpHeight) {
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

void Renderer::renderScene(Scene& scene) {
    //shadow pass
    renderShadowMap(scene);

    

    GL(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
    glViewport(0, 0, m_vpWidth, m_vpHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glEnable(GL_BLEND); 
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (auto& model : scene.getModels()) {
        auto& modelProgram = model->getProgram();
        modelProgram.bind();
        GL(glBindTexture(GL_TEXTURE_2D, m_depthMap));
        GL(glActiveTexture(GL_TEXTURE0));
        modelProgram.setUniformInt("u_shadowMap", 0);


        renderModel(*model);
        modelProgram.unbind();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Renderer::getImage() const { return m_image; }

Renderer::~Renderer() {
    if (glIsTexture(m_image)) glDeleteTextures(1, &m_image);
    if (glIsRenderbuffer(m_RBO)) glDeleteRenderbuffers(1, &m_RBO);
    if (glIsFramebuffer(m_FBO)) glDeleteFramebuffers(1, &m_FBO);
}

#pragma region ** STATICS **

void Renderer::bindModel(Model& model) {
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

void Renderer::unbindObject(Model& model) {
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

void Renderer::enableDepthMask() { glDepthMask(GL_TRUE); }
void Renderer::enableBackfaceCull() { glEnable(GL_CULL_FACE); }

void Renderer::disableDepthMask() { glDepthMask(GL_FALSE); }
void Renderer::disableBackfaceCull() { glDisable(GL_CULL_FACE); }

#pragma endregion


void Renderer::renderModel(Model& model) {
    GL(glBindVertexArray(model.getMesh()->VAO));

    GL(glDrawElements(GL_TRIANGLES, model.getMesh()->indices.size(), GL_UNSIGNED_INT, 0));

    glBindVertexArray(0);
}

void Renderer::renderShadowMap(Scene& scene) {
    

    GL(glBindFramebuffer(GL_FRAMEBUFFER, m_depthFBO));
    glViewport(0, 0, m_depthMapWidth, m_depthMapHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    m_shadowMapProgram.bind();
    glm::mat4 lightSpaceMatrix = scene.getLights().front()->getLightSpaceMatrix();
    m_shadowMapProgram.setUniformMat4("u_lightSpaceMatrix", lightSpaceMatrix);

    for (auto& model : scene.getModels()) {
        m_shadowMapProgram.setUniformMat4("u_model", model->getModelMatrix());
        renderModel(*model);
    }

    m_shadowMapProgram.unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}