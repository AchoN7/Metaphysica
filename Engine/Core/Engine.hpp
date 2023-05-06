#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Core/Window.hpp"
#include "GUI/GUI.hpp"

#include "Graphics/Renderer.hpp"
#include "Scene/Scene.hpp"

class Engine {
public:

	Engine();
	~Engine();

	void mainLoop();
	
private:

	Window m_window;
	GUI m_GUI;
	Renderer m_renderer;

	Scene m_activeScene;

	float m_deltaTime;
	float m_lastFrame;

	void inputs();

	void update();

	void render();

	void updateDimensions(int windowWidth, int windowHeight);

	void processCursor(double xposIn, double yposIn);

	void processScroll(double yoffset);

	//////////GLFW CALLBACKS
	static void windowSizeCallback(GLFWwindow* window, int width, int height);

	static void cursorCallback(GLFWwindow* window, double xposIn, double yposIn);

	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

};