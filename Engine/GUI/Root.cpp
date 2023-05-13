#include "Root.hpp"

using namespace ImGui;

Root::Root(int width, int height) {
    m_position = ImVec2(0, 0);
    m_dimensions = ImVec2(width, height);
    m_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;

    m_bgColor = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
}

void Root::display() {

    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    PushStyleColor(ImGuiCol_WindowBg, m_bgColor);
    SetNextWindowPos(m_position);
    SetNextWindowSize(m_dimensions);
    Begin("Root", nullptr, m_flags);
    //DockSpaceOverViewport(GetMainViewport(), ImGuiDockNodeFlags_None);

    ImGuiID dockspace_id = GetID("RootDockspace");
    DockSpace(dockspace_id);

    if (BeginMenuBar()){
        if (BeginMenu("File")){
            if (MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
            if (MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
            EndMenu();
        }
        EndMenuBar();
    }



    End();
    PopStyleColor();
    PopStyleVar();
}

void Root::setDimensions(int width, int height) {
    m_position = ImVec2(0, 0);
    m_dimensions = ImVec2(width, height);
}