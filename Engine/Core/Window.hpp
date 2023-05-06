#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

class Window {
public:

    Window();
    ~Window();
    
    void updateDimensions(int windowWidth, int windowHeight);
    void swapBuffers() const;

    GLFWwindow* getWindow() const;
    GLFWmonitor* getMonitor() const;
    int getWidth() const;
    int getHeight() const;
    int getXpos() const;
    int getYpos() const;

private:

	GLFWwindow* m_window;
    GLFWmonitor* m_monitor;

    int m_width;
    int m_height;

    int m_Xpos;
    int m_Ypos;

};