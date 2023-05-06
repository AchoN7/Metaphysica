#pragma once

#include "imgui.h"

#include "Graphics/Renderer.hpp"
#include "Scene/Scene.hpp"

using namespace ImGui;

class ModelCreator {
public:

	ModelCreator() : m_renderer(), m_scene() {
		m_minSize = ImVec2(1280, 720);
		m_renderer.recreateFramebuffer(m_minSize.x, m_minSize.y);
		m_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
	}

	~ModelCreator();

	void updateDimensions();

	void display() {

		SetNextWindowSizeConstraints(m_minSize, ImVec2(4000, 2000));
		Begin("Model Creator", nullptr, m_flags);

		//menu bar with options to save, undo, redo..

		ImVec2 regionSize = GetContentRegionAvail();

		BeginTabBar("Tabs");
		SetNextItemWidth(regionSize.x / 3);
		if (BeginTabItem("List", nullptr)) {
			//shows list of active shapes in the mini scene
			//selectable, shows shape modification options like color, material, size..
			EndTabItem();
		}
		SetNextItemWidth(regionSize.x / 3);
		if (BeginTabItem("Shapes", nullptr)) {
			//static square images of shapes (cube, sphere for now)
			//drag and drop 
			EndTabItem();
		}
		SetNextItemWidth(regionSize.x / 3);
		if (BeginTabItem("Settings", nullptr)) {
			//global mini scene settings like light options, show grid on/off
			//tickboxes for rendering points, lines, or normals of the shapes
			EndTabItem();
		}
		EndTabBar();


		End();

	}

private:

	Scene m_scene;
	Renderer m_renderer;

	ImVec2 m_minSize;
	unsigned int m_flags;

};