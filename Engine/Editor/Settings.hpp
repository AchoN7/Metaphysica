#pragma once

#include "GUI/GUI.hpp"

#include "Core/Logger.hpp"
#include "Scene/Scene.hpp"
 
class Settings : public GUIelement {
public:

    Settings(Scene& sceneRef, Renderer& rendererRef);

    virtual void onGUIdisplay();

private:

    ImGuiWindowFlags m_flags;
    Scene& m_sceneRef;
    Renderer& m_rendererRef;

    void lightProperties();
    void renderingOptions();

};