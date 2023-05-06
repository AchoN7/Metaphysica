#pragma once

#include "imgui.h"

class Viewport {
public:

    Viewport();

    void update();
    void display(unsigned int image);

    int getWidth() const;
    int getHeight() const;

    bool isFocused() const;
    bool isResized() const;
    bool isHovered() const;
    bool isRightClicked() const;
    void clearResizeFlag();

private:

    ImVec2 m_dimensions;
    unsigned int m_flags;
    bool m_isFocused;
    bool m_isResized;
    bool m_isHovered;
    bool m_isRightClicked;

};