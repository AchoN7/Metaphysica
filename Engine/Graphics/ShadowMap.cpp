#include "ShadowMap.hpp"

#include "Core/GLdebug.hpp"

ShadowMap::ShadowMap() :
    m_program(),
    m_depthFBO(0),
    m_depthMap(0),
    m_scale(4),
    m_depthMapWidth(m_scale * 1024),
    m_depthMapHeight(m_scale * 1024)
{

    recreate();

    m_program.attachShader(ShaderType::VERTEX, "Shaders/Shadow.vert");
    m_program.attachShader(ShaderType::FRAGMENT, "Shaders/Shadow.frag");
    m_program.link();
}

void ShadowMap::shadowPass(Scene& scene) {
    GL(glBindFramebuffer(GL_FRAMEBUFFER, m_depthFBO));
    glViewport(0, 0, m_depthMapWidth, m_depthMapHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    m_program.bind();
    glm::mat4 lightSpaceMatrix = scene.getLight()->getLightSpaceMatrix();
    m_program.setUniformMat4("u_lightSpaceMatrix", lightSpaceMatrix);

    for (auto& model : scene.getModels()) {
        m_program.setUniformMat4("u_model", model->getModelMatrix());

        GL(glBindVertexArray(model->getMesh()->VAO));
        GL(glDrawElements(GL_TRIANGLES, model->getMesh()->indices.size(), GL_UNSIGNED_INT, 0));
        glBindVertexArray(0);
    }

    m_program.unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::bind() {
    GL(glBindTexture(GL_TEXTURE_2D, m_depthMap));
    GL(glActiveTexture(GL_TEXTURE0));
}

void ShadowMap::setResolutionScale(int scale) {
    m_depthMapWidth = 1024 * scale;
    m_depthMapHeight = 1024 * scale;

    recreate();
}

void ShadowMap::recreate() {
    // delete previous objects from GPU
    if (glIsTexture(m_depthMap)) glDeleteTextures(1, &m_depthMap);
    if (glIsFramebuffer(m_depthFBO)) glDeleteFramebuffers(1, &m_depthFBO);

    //create resources for shadow mapping
    glGenFramebuffers(1, &m_depthFBO);
    glGenTextures(1, &m_depthMap);

    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_depthMapWidth, m_depthMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //glm::vec4 borderColor(1.0f);
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));

    glBindFramebuffer(GL_FRAMEBUFFER, m_depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
