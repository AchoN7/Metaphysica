#pragma once

#include <GL/glew.h>

#include "Scene/Scene.hpp"

class Renderer {
public:

    Renderer();
    ~Renderer();

    void recreateFramebuffer(int vpWidth, int vpHeight);
    void renderScene(Scene& scene);

    GLuint getImage() const;

#pragma region ** STATICS **

    static void bindModel(Model& model);
    static void unbindObject(Model& model);

#pragma endregion

#pragma region ** OPENGL SWITCHES **

    void enableDepthMask();
    void enableBackfaceCull();

    void disableDepthMask();
    void disableBackfaceCull();

#pragma endregion

private:

	GLuint m_image;
	GLuint m_FBO;
    GLuint m_RBO;

    Shader m_shadowMapProgram;
    GLuint m_depthFBO;
    GLuint m_depthMap;
    unsigned int m_depthMapWidth;
    unsigned int m_depthMapHeight;
	
    int m_vpWidth;
    int m_vpHeight;

    void renderModel(Model& model);

    void renderShadowMap(Scene& scene);
	
};

