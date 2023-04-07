#pragma once

#include <GLFW/glfw3.h>

class Window {
public:

    Window() {

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        m_monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* videoMode = glfwGetVideoMode(m_monitor);

        m_width = videoMode->width * 0.8;
        m_height = videoMode->height * 0.8;
        m_Xpos = (videoMode->width - m_width) / 2;
        m_Ypos = (videoMode->height - m_height) / 2;

        m_window = glfwCreateWindow(m_width, m_height, "Metaphysica", nullptr, nullptr);
        glfwSetWindowAspectRatio(m_window, m_width, m_height);
        glfwSetWindowSizeLimits(m_window, m_width, m_height, GLFW_DONT_CARE, GLFW_DONT_CARE);
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1);
    }

    void shutdown() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
    
    void updateDimensions(int windowWidth, int windowHeight) {
        m_width = windowWidth;
        m_height = windowHeight;

        glfwGetWindowPos(m_window, &m_Xpos, &m_Ypos);
    }

    void swapBuffers() const { glfwSwapBuffers(m_window); }
    GLFWwindow* getWindow() const { return m_window; }
    GLFWmonitor* getMonitor() const { return m_monitor; }

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getXpos() const { return m_Xpos; }
    int getYpos() const { return m_Ypos; }

private:

	GLFWwindow* m_window;
    GLFWmonitor* m_monitor;

    int m_width;
    int m_height;

    int m_Xpos;
    int m_Ypos;

};