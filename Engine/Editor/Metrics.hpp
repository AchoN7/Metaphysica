#pragma once

#include "GUI/GUI.hpp"

#include "Core/Logger.hpp"

using namespace ImGui;

class Metrics : public GUIelement {
public:

    Metrics() {

    }

    virtual void onGUIdisplay() override {
        Begin("Metrics");

        End();
    }

private:
    ImGuiWindowFlags m_flags;
};