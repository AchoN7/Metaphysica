#pragma once

#include <imgui.h>
#define GL_SILENCE_DEPRECATION

#include "Core/Logger.hpp"

using namespace ImGui;

class Metrics {
public:

    Metrics() {

    }

    void display() {
        //current scene time
        //fps 
        //number of triangles
        //number of objects
    }

private:
    ImGuiWindowFlags m_flags;
};