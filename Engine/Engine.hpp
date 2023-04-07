#pragma once

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GUI/Window.hpp"
#include "GUI/GUI.hpp"
#include "Graphics\Renderer.hpp"

class Engine {
public:

	Engine() : m_window(), m_GUI(m_window), m_renderer(m_GUI.getViewportWidth(), m_GUI.getViewportHeight()) {
		glfwSetWindowUserPointer(m_window.getWindow(), this);
		glfwSetWindowSizeCallback(m_window.getWindow(), Engine::windowSizeCallback);
	}

	void shutdown() {
		m_GUI.shutdown();
		m_window.shutdown();
	}

	void mainLoop() {
		
		while (!glfwWindowShouldClose(m_window.getWindow())) {
			inputs();
			update();
			render();
		}
	}
	
private:

	Window m_window;
	GUI m_GUI;
	Renderer m_renderer;

	void inputs() {
		glfwPollEvents();
	}

	void update() {
		if (m_GUI.isViewportResized()) {
			m_renderer.recreateFramebuffer(m_GUI.getViewportWidth(), m_GUI.getViewportHeight());
			m_GUI.clearViewportResizeFlag();
		}

	}

	void render() {
		glViewport(0, 0, m_GUI.getViewportWidth(), m_GUI.getViewportHeight());

		m_renderer.render();

		m_GUI.render(m_renderer.getImage());

		m_window.swapBuffers();
	}

	void updateDimensions(int windowWidth, int windowHeight) {
		m_window.updateDimensions(windowWidth, windowHeight);
		m_GUI.updateDimensions(windowWidth, windowHeight);
		
	}

	static void windowSizeCallback(GLFWwindow* window, int width, int height) {
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		engine->updateDimensions(width, height);

		//single update/render sequence to update screen contents while resizing
		engine->update();
		engine->render();
	}

};