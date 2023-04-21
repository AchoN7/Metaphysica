#pragma once

#include <imgui.h>
#define GL_SILENCE_DEPRECATION

#include "Graphics/Renderer.hpp"

#include "Core/Logger.hpp"

using namespace ImGui;

class Settings {
public:

    Settings(Renderer& rendererRef) : m_rendererRef(rendererRef) {}

    void display() {

        //global options like changing sun position, color, intensity
        //physics values like G, Time slow/speed up

        //selected object options like change physical properties

    }

private:
    ImGuiWindowFlags m_flags;

    Renderer& m_rendererRef;



};