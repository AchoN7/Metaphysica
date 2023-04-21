#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Core/Window.hpp"
#include "GUI/GUI.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Renderer.hpp"
#include "World/World.hpp"
 

class Engine {
public:

	Engine() : m_window(), m_renderer(), m_GUI(m_window, m_renderer), m_world()
	{
		glfwSetWindowUserPointer(m_window.getWindow(), this);
		glfwSetWindowSizeCallback(m_window.getWindow(), Engine::windowSizeCallback);
		//glfwSetCursorPosCallback(m_window.getWindow(), Engine::cursorCallback);
		glfwSetScrollCallback(m_window.getWindow(), Engine::scrollCallback);

		m_renderer.recreateFramebuffer(m_GUI.getViewportWidth(), m_GUI.getViewportHeight());
		m_renderer.getStarProgram().attachShader(ShaderType::VERTEX, "Shaders/vx_Star.shader");
		m_renderer.getStarProgram().attachShader(ShaderType::FRAGMENT, "Shaders/fr_Star.shader");
		m_renderer.getStarProgram().link();

		m_renderer.getModelProgram().attachShader(ShaderType::VERTEX, "Shaders/vx_Model.shader");
		m_renderer.getModelProgram().attachShader(ShaderType::FRAGMENT, "Shaders/fr_Model.shader");
		m_renderer.getModelProgram().link();

		m_world.getCamera().onViewportResized(m_GUI.getViewportWidth(), m_GUI.getViewportHeight());
		m_renderer.bindModel(m_world.getGround());
		m_renderer.bindModel(m_world.getStar());
		for (auto& model : m_world.getModels()) {
			m_renderer.bindModel(model);
		}

	}

	void shutdown() {
		m_renderer.shutdown();
		m_GUI.shutdown();
		m_window.shutdown();
	}

	void mainLoop() {

		while (!glfwWindowShouldClose(m_window.getWindow())) {

			float currentFrame = static_cast<float>(glfwGetTime());
			m_deltaTime = currentFrame - m_lastFrame;
			m_lastFrame = currentFrame;

			inputs();
			update();
			render();

		}
	}
	
private:

	Window m_window;
	GUI m_GUI;
	Renderer m_renderer;
	World m_world;

	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;

	void inputs() {
		//TODO: encapsulate in own class?
		//TODO: make sure to move if the mouse is on the viewport
		glfwPollEvents();

		if (glfwGetKey(m_window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
			m_world.getCamera().move(Camera::Direction::FORWARD, m_deltaTime);
		if (glfwGetKey(m_window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
			m_world.getCamera().move(Camera::Direction::BACKWARD, m_deltaTime);
		if (glfwGetKey(m_window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
			m_world.getCamera().move(Camera::Direction::LEFT, m_deltaTime);
		if (glfwGetKey(m_window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
			m_world.getCamera().move(Camera::Direction::RIGHT, m_deltaTime);
	}

	void update() {
		if (m_GUI.isViewportResized()) {
			m_renderer.recreateFramebuffer(m_GUI.getViewportWidth(), m_GUI.getViewportHeight());
			m_world.getCamera().onViewportResized(m_GUI.getViewportWidth(), m_GUI.getViewportHeight());
			m_GUI.clearViewportResizeFlag();
		}

		//TEMP
		if (m_GUI.isViewportFocused() && m_GUI.isViewportHovered()) {
			double xpos, ypos;
			glfwGetCursorPos(m_window.getWindow(), &xpos, &ypos);
			processCursor(xpos, ypos);
		}

		m_world.update(m_deltaTime);

	}

	void render() {	

		m_renderer.renderWorld(m_world);

		m_GUI.render(m_renderer.getImage());

		m_window.swapBuffers();
	}

	void updateDimensions(int windowWidth, int windowHeight) {
		m_window.updateDimensions(windowWidth, windowHeight);
		m_GUI.updateDimensions(windowWidth, windowHeight);
		
	}

	void processCursor(double xposIn, double yposIn) {
		static bool firstMouse = true;
		static float lastX = static_cast<float>(m_GUI.getViewportWidth() / 2.0f);
		static float lastY = static_cast<float>(m_GUI.getViewportHeight() / 2.0f);

		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		if (m_GUI.isViewportHovered() && m_GUI.isViewportRightClicked()) {
			m_world.getCamera().mouseLook(xoffset, yoffset, true);
		}
		
			
	}

	void processScroll(double yoffset) {
		m_world.getCamera().scrollZoom(static_cast<float>(yoffset));
	}

	//////////GLFW CALLBACKS
	static void windowSizeCallback(GLFWwindow* window, int width, int height) {
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		engine->updateDimensions(width, height);

		//single update/render sequence to update screen contents while resizing
		engine->update();
		engine->render();
	}

	static void cursorCallback(GLFWwindow* window, double xposIn, double yposIn) {
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		engine->processCursor(xposIn, yposIn);
	}

	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		engine->processScroll(yoffset);
	}

};