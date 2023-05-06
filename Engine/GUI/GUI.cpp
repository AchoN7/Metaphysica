#include "GUI.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#define GL_SILENCE_DEPRECATION

#include "Core/Logger.hpp"

using namespace ImGui;

GUI::GUI(const Window& window, Renderer& rendererRef, Scene& sceneRef) 
    : m_windowRef(window), m_rendererRef(rendererRef), m_editor(sceneRef),
    m_root(window.getWidth(), window.getHeight()) {

    IMGUI_CHECKVERSION();
    CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
    StyleColorsDark();

};

GUI::~GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    DestroyContext();
}

void GUI::update() {

}

void GUI::display(unsigned int image) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    NewFrame();
    //=============================================

    m_root.display();
    m_viewport.display(image);
    m_editor.display();
    


    //=============================================
    Render();
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}

void GUI::updateDimensions(int windowWidth, int windowHeight) {
    m_root.setDimensions(windowWidth, windowHeight);
}
