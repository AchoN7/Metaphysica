#include "Settings.hpp"

using namespace ImGui;

Settings::Settings(Scene& sceneRef, Renderer& rendererRef) 
    : m_sceneRef(sceneRef), m_rendererRef(rendererRef) {}

void Settings::onGUIdisplay() {

    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 10.0f));
    Begin("Settings");

    lightProperties();
    Separator();
    renderingOptions();
    Separator();




    //selected object options like change physical properties
    //if (Button("Stop Physics")) { m_sceneRef.pausePhysics(); }
    //if (Button("Resume Physics")) { m_sceneRef.resumePhysics(); }
    //if (Button("Speed up time")) { m_sceneRef.speedupTime(); }
    //if (Button("Normal time")) { m_sceneRef.normalTime(); }

    End();
    PopStyleVar(1);
}

void Settings::lightProperties() {
    static auto& sceneMainLight = *m_sceneRef.getLight();

    //color properties
    Text("Light properties:");
    static auto lightColor = sceneMainLight.getColor();
    static float lightColorChange[3]{
        lightColor.r,
        lightColor.g,
        lightColor.b
    };

    if (ColorEdit3("Light Color", lightColorChange, ImGuiColorEditFlags_Float)) {
        glm::vec4 newColor = glm::vec4(lightColorChange[0], lightColorChange[1], lightColorChange[2], 1.0f);
        sceneMainLight.setColor(newColor);
    }

    //position
    static auto lightPos = sceneMainLight.getPosition();
    static float lightPosChange[3]{
        lightPos.x,
        lightPos.y,
        lightPos.z
    };

    if (SliderFloat3("Light position", lightPosChange, -20.0f, 20.0f)) {
        sceneMainLight.setPosition(glm::vec3(lightPosChange[0], lightPosChange[1], lightPosChange[2]));
    }
}

void Settings::renderingOptions() {
    Text("Rendering options:");
    
    static bool shouldWireframe = false;
    static bool shouldNormals = false;
    static bool shouldPoints = false;
    static bool shouldVSync = false;

    if (Checkbox("Wireframe mode", &shouldWireframe)) {
        if (shouldWireframe) m_rendererRef.turnOnWireframe();
        else m_rendererRef.turnOffWireframe();
    }
    if (Checkbox("Show normals", &shouldNormals)) {

    }
    if (Checkbox("Show points", &shouldPoints)) {

    }
    if (Checkbox("Vsync on", &shouldVSync)) {

    }

    Text("Shadow options:");
    static int depthMapResolutionScale = m_rendererRef.getShadowMap().getResolutionScale();
    if (SliderInt("Resolution scale", &depthMapResolutionScale, 1, 16))
        m_rendererRef.getShadowMap().setResolutionScale(depthMapResolutionScale);
}
    