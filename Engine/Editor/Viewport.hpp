#pragma once

#include "GUI/GUI.hpp"

class Viewport : public GUIelement {
public:

    Viewport(const Renderer& rendererRef);

    void update();
    void display();

    virtual void onGUIdisplay() override;

    int getWidth() const;
    int getHeight() const;

    bool isFocused() const;
    bool isResized() const;
    bool isHovered() const;
    bool isRightClicked() const;
    void clearResizeFlag();

private:

    const Renderer& m_rendererRef;

    ImVec2 m_dimensions;
    unsigned int m_image;
    unsigned int m_flags;
    bool m_isFocused;
    bool m_isResized;
    bool m_isHovered;
    bool m_isRightClicked;

};