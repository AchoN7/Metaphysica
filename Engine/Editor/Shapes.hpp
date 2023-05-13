#pragma once

#include "GUI/GUI.hpp"

class Shapes : public GUIelement {
public:

	Shapes(/*renderer ref to render the images one time and save them*/); //

	virtual void onGUIdisplay() override;

private:

	//small images for each shape
	std::vector<unsigned int> m_shapeImages;

};