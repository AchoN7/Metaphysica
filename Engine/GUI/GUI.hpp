#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#define GL_SILENCE_DEPRECATION

#include "Window.hpp"
#include "Console.hpp"
#include "Core/Logger.hpp"


using namespace ImGui;

class GUI {
public:

	GUI(const Window& window) : m_windowRef(window)  {
        CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
        StyleColorsDark();

        m_rootPosition = ImVec2(0, 0);
        m_rootDimensions = ImVec2(window.getWidth(), window.getHeight());
        m_rootFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;

        m_viewportDimensions = ImVec2(800, 600);
        m_viewportFlags =  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
        m_viewportResized = false;
        m_viewportHovered = false;
        m_viewportRightClicked = false;
    };

    void shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        DestroyContext();
    }

    void update() {

    }

	void render(unsigned int image) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        NewFrame();

        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
        SetNextWindowPos(m_rootPosition);
        SetNextWindowSize(m_rootDimensions);
        Begin("Root", nullptr, m_rootFlags);

        //TODO: put inside Menu
        Console::renderConsole();

        renderViewport(image);
        //m_menu.renderMenu();

        End();
        PopStyleColor();
        PopStyleVar();

        Render();
        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
	}

    void renderViewport(unsigned int image) {

        SetNextWindowSizeConstraints(ImVec2(800, 619), ImVec2(10000, 7000));
        Begin("Viewport", nullptr, m_viewportFlags);

        ImVec2 currentRegionSize = GetContentRegionAvail();
        if (m_viewportDimensions.x != currentRegionSize.x || m_viewportDimensions.y != currentRegionSize.y) {
            m_viewportDimensions = currentRegionSize;
            m_viewportResized = true;
        }

        m_viewportHovered = IsWindowHovered();
        m_viewportRightClicked = IsMouseDown(ImGuiMouseButton_Right);
        

        //TODO: have this Text in the Metrics window
        //Text("Res: %d, %d", (int)m_viewportDimensions.x, (int)m_viewportDimensions.y);

        Image((ImTextureID)(intptr_t)image, m_viewportDimensions, ImVec2(0, 1), ImVec2(1, 0));
      
        End();
    }

    void updateDimensions(int windowWidth, int windowHeight) {
        m_rootPosition = ImVec2(0, 0);
        m_rootDimensions = ImVec2(windowWidth, windowHeight);
    }

    int getViewportWidth() const { return static_cast<int>(m_viewportDimensions.x); }
    int getViewportHeight() const { return static_cast<int>(m_viewportDimensions.y); }

    bool isViewportResized() const { return m_viewportResized; }
    bool isViewportHovered() const { return m_viewportHovered; }
    bool isViewportRightClicked() const { return m_viewportRightClicked; }
    void clearViewportResizeFlag() { m_viewportResized = false; }

private:

    const Window& m_windowRef;
    
    //Viewport m_viewport;
    //Menu m_menu;

    ImVec2 m_rootPosition;
    ImVec2 m_rootDimensions;
    unsigned int m_rootFlags;

    ImVec2 m_viewportDimensions;
    unsigned int m_viewportFlags;
    bool m_viewportResized;
    bool m_viewportHovered;
    bool m_viewportRightClicked;

};