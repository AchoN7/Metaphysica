#pragma once

#include <GL/glew.h>

#include "Scene/Scene.hpp"
#include "ShadowMap.hpp"

class Renderer {
public:

    Renderer();
    ~Renderer();

    void recreateFramebuffer(int vpWidth, int vpHeight);
    void renderScene(Scene& scene);

    GLuint getImage() const;
    ShadowMap& getShadowMap();

#pragma region ** STATICS **

    static void bindModel(Model& model);
    static void unbindObject(Model& model);

#pragma endregion

#pragma region ** OPENGL SWITCHES **

    void enableDepthMask();
    void enableBackfaceCull();

    void disableDepthMask();
    void disableBackfaceCull();

    void turnOnWireframe();
    void turnOffWireframe();

#pragma endregion

private:

	GLuint m_image;
	GLuint m_FBO;
    GLuint m_RBO;

    int m_vpWidth;
    int m_vpHeight;

    ShadowMap m_shadowMap;

    void renderModel(Model& model);
	
};

