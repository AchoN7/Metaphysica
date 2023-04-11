#pragma once

#include <imgui.h>
#define GL_SILENCE_DEPRECATION

#include "Graphics/Renderer.hpp"

#include "Core/Logger.hpp"

using namespace ImGui;

class Settings {
public:

    Settings(Renderer& rendererRef) : m_rendererRef(rendererRef) {

    }

    void display() {
 
        Indent();
        Text("Wireframe mode: ");
        SameLine();
        if (Button("ON"))
            m_rendererRef.wireframeMode(true);
        
        SameLine();
        Separator();
        SameLine();

        if (Button("OFF"))
            m_rendererRef.wireframeMode(false);
    }

private:
    ImGuiWindowFlags m_flags;

    Renderer& m_rendererRef;
};