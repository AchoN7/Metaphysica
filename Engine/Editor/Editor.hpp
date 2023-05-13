#pragma once

#include "GUI/GUI.hpp"

using namespace ImGui;

class Editor : public GUIelement {
public:

    Editor(Scene& sceneRef) {}


	//void display() {
 //       if (Begin("Scene Editor", nullptr, ImGuiWindowFlags_NoCollapse)) {
 //           ImVec2 regionSize = GetContentRegionAvail();
 //           float tabWidth = regionSize.x / 4;

 //           BeginTabBar("Tabs");
 //           SetNextItemWidth(tabWidth);
 //           if (BeginTabItem("Scene", nullptr)) {
 //               //m_sceneSettings.display();
 //               EndTabItem();
 //           }
 //           SetNextItemWidth(tabWidth);
 //           if (BeginTabItem("Console", nullptr)) {
 //               m_console.display();
 //               EndTabItem();
 //           }
 //           SetNextItemWidth(tabWidth);
 //           if (BeginTabItem("Metrics", nullptr)) {
 //               //m_assetsManager.display();
 //               EndTabItem();
 //           }
 //           EndTabBar();

 //           End();
 //       }
	//}

    virtual void onGUIdisplay() override {
        Begin("Editor");

        End();
    }

private:


};
