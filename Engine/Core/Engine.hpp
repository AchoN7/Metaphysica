#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Core/Window.hpp"
#include "GUI/GUI.hpp"

#include "Graphics/Renderer.hpp"
#include "Scene/Scene.hpp"

#include "Editor/Editor.hpp"
#include "Editor/Viewport.hpp"
#include "Editor/Metrics.hpp"
#include "Editor/Settings.hpp"
#include "Editor/Shapes.hpp"
#include "Editor/Console.hpp"

class Engine {
public:

	Engine();
	~Engine();

	void mainLoop();
	
private:

	Window m_window;
	GUI m_GUI;
	Renderer m_renderer;

	std::unique_ptr<Editor> m_editor;
	std::unique_ptr<Viewport> m_viewport;
	std::unique_ptr<Metrics> m_metrics;
	std::unique_ptr<Settings> m_settings;
	std::unique_ptr<Shapes> m_shapes;
	std::unique_ptr<Console> m_console;

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