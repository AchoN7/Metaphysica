#pragma once

#include "GL/glew.h"
#include "Shader.hpp"
#include "Scene/Scene.hpp"

class ShadowMap {
public:

    ShadowMap();
    
    void shadowPass(Scene& scene);
    void bind();

    GLuint getDepthMap() const { return m_depthMap; }
    int getResolutionScale() const { return m_scale; }

    void setResolutionScale(int scale);

private:

    Shader m_program;
    GLuint m_depthFBO;
    GLuint m_depthMap;
    
    int m_scale;
    unsigned int m_depthMapWidth;
    unsigned int m_depthMapHeight;
    

    void recreate();

};