#include "Viewport.hpp"

using namespace ImGui;

Viewport::Viewport(const Renderer& rendererRef) : m_rendererRef(rendererRef) {
    m_dimensions = ImVec2(800, 600);
    m_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
    m_isFocused = true;
    m_isResized = false;
    m_isHovered = false;
    m_isRightClicked = false;
}

void Viewport::update() {
    m_image = m_rendererRef.getImage();
}

void Viewport::onGUIdisplay() {

    SetNextWindowSizeConstraints(ImVec2(800, 619), ImVec2(10000, 7000));
    Begin("Viewport", nullptr, m_flags);

    ImVec2 currentRegionSize = GetContentRegionAvail();
    if (m_dimensions.x != currentRegionSize.x || m_dimensions.y != currentRegionSize.y) {
        m_dimensions = currentRegionSize;
        m_isResized = true;
    }

    m_isFocused = IsWindowFocused();
    m_isHovered = IsWindowHovered();
    m_isRightClicked = IsMouseDown(ImGuiMouseButton_Right);

    Image((ImTextureID)(intptr_t)m_image, m_dimensions, ImVec2(0, 1), ImVec2(1, 0));

    End();
}

int Viewport::getWidth() const { return static_cast<int>(m_dimensions.x); }
int Viewport::getHeight() const { return static_cast<int>(m_dimensions.y); }

bool Viewport::isFocused() const { return m_isFocused; }
bool Viewport::isResized() const { return m_isResized; }
bool Viewport::isHovered() const { return m_isHovered; }
bool Viewport::isRightClicked() const { return m_isRightClicked; }
void Viewport::clearResizeFlag() { m_isResized = false; }