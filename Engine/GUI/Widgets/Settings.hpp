#pragma once

#include <imgui.h>
#define GL_SILENCE_DEPRECATION

#include "Core/Logger.hpp"
#include "Scene/Scene.hpp"

using namespace ImGui;

class Settings {
public:

    Settings(Scene& sceneRef) : m_sceneRef(sceneRef) {}

    void display() {

        //global options like changing sun position, color, intensity
        //physics values like G, Time slow/speed up

        //selected object options like change physical properties
        if (Button("Stop Physics")) { m_sceneRef.pausePhysics(); }
        if (Button("Resume Physics")) { m_sceneRef.resumePhysics(); }
    }

private:

    ImGuiWindowFlags m_flags;

    Scene& m_sceneRef;



};