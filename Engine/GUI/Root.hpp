#pragma once
#include <imgui.h>

class Root {
public:

	Root(int width, int height);

	void display();
	void setDimensions(int width, int height);

private:

	ImVec2 m_position;
	ImVec2 m_dimensions;
	ImVec4 m_bgColor;
	unsigned int m_flags;

};