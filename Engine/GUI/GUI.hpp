#pragma once

#include <imgui.h>
#include "Core/Window.hpp"

#include "Root.hpp"
#include "Viewport.hpp"
#include "Editor.hpp"

#include "Widgets/Settings.hpp"
#include "Widgets/Console.hpp"
#include "Widgets/Metrics.hpp" 

#include "Graphics/Renderer.hpp"
#include "Scene/Scene.hpp"


class GUI {
public:

    GUI(const Window& window, Renderer& rendererRef, Scene& sceneRef);

    ~GUI();

    void update();

    void display(unsigned int image);

    void updateDimensions(int windowWidth, int windowHeight);

    auto& getViewport() { return m_viewport; }

private:

    const Window& m_windowRef;
    Renderer& m_rendererRef;
   
    Root m_root;
    Viewport m_viewport;
    Editor m_editor;

};