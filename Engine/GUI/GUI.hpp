#pragma once

#include <imgui.h>
#include "Core/Window.hpp"

#include "Root.hpp"

#include "Graphics/Renderer.hpp"
#include "Scene/Scene.hpp"

class GUIelement;

class GUI {
public:

    GUI(const Window& window);

    ~GUI();

    void addGUIelement(GUIelement* element);

    void update();

    void display();

    void updateDimensions(int windowWidth, int windowHeight);


private:

    const Window& m_windowRef;
   
    Root m_root;

    // GUI windows like the Viewport, Content browser, Console, Metrics, Editor etc..
    std::vector<GUIelement*> m_GUIelements;

};

class GUIelement {
public:
    //virtual void onGUIupdate() = 0;
    virtual void onGUIdisplay() = 0;
};