#pragma once

#include "Widgets/Settings.hpp"
#include "Widgets/Assets.hpp"
#include "Widgets/Console.hpp"
#include "Widgets/Metrics.hpp"

class Editor {
public:

    Editor(Scene& sceneRef) : m_settings(sceneRef) {}


	void display() {
        if (Begin("Editor", nullptr, ImGuiWindowFlags_NoCollapse)) {
            ImVec2 regionSize = GetContentRegionAvail();

            BeginTabBar("Tabs");
            SetNextItemWidth(regionSize.x / 3);
            if (BeginTabItem("Settings", nullptr)) {
                m_settings.display();
                EndTabItem();
            }
            SetNextItemWidth(regionSize.x / 3);
            if (BeginTabItem("Console", nullptr)) {
                m_console.display();
                EndTabItem();
            }
            SetNextItemWidth(regionSize.x / 3);
            if (BeginTabItem("Metrics", nullptr)) {
                m_metrics.display();
                EndTabItem();
            }
            SetNextItemWidth(regionSize.x / 3);
            if (BeginTabItem("Assets", nullptr)) {
                m_assetsManager.display();
                EndTabItem();
            }
            EndTabBar();

            End();
        }
	}

private:

	//SceneManager m_sceneManager;
	Assets m_assetsManager;
	Console m_console;
    Settings m_settings;
    Metrics m_metrics;

};

/* Main Tabs: 

	Scene, Assets, Console (only when debugging)

	Scene:
		tabs: Manager

*/