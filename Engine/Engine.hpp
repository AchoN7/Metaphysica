#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Core/Window.hpp"
#include "GUI/GUI.hpp"

#include "Graphics/Camera.hpp"
#include "Graphics/Renderer.hpp"

#include "Objects/Sphere.hpp"

class Engine {
public:

	Engine() : m_window(), m_renderer(), m_GUI(m_window, m_renderer), m_camera(),
		m_sphere(glm::vec4(0.4f, 0.1f, 0.96f, 1.0f), SphereQuality::HIGH) 
	{
		glfwSetWindowUserPointer(m_window.getWindow(), this);
		glfwSetWindowSizeCallback(m_window.getWindow(), Engine::windowSizeCallback);
		//glfwSetCursorPosCallback(m_window.getWindow(), Engine::cursorCallback);
		glfwSetScrollCallback(m_window.getWindow(), Engine::scrollCallback);

		m_renderer.recreateFramebuffer(m_GUI.getViewportWidth(), m_GUI.getViewportHeight());
		m_renderer.bindSphere(m_sphere);
	}

	void shutdown() {
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
	Camera m_camera;
	Sphere m_sphere; //temp

	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;

	void inputs() {
		//TODO: encapsulate in own class?
		//TODO: make sure to move if the mouse is on the viewport
		glfwPollEvents();

		if (glfwGetKey(m_window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
			m_camera.move(Camera::Direction::FORWARD, m_deltaTime);
		if (glfwGetKey(m_window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
			m_camera.move(Camera::Direction::BACKWARD, m_deltaTime);
		if (glfwGetKey(m_window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
			m_camera.move(Camera::Direction::LEFT, m_deltaTime);
		if (glfwGetKey(m_window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
			m_camera.move(Camera::Direction::RIGHT, m_deltaTime);
	}

	void update() {
		if (m_GUI.isViewportResized()) {
			m_renderer.recreateFramebuffer(m_GUI.getViewportWidth(), m_GUI.getViewportHeight());
			m_GUI.clearViewportResizeFlag();
		}

		//TEMP
		if (m_GUI.isViewportHovered()) {
			double xpos, ypos;
			glfwGetCursorPos(m_window.getWindow(), &xpos, &ypos);
			processCursor(xpos, ypos);
		}
			
	}

	void render() {
		glViewport(0, 0, m_GUI.getViewportWidth(), m_GUI.getViewportHeight());

		mat4 model = mat4(1.0f);
		mat4 view = m_camera.getViewMatrix();
		mat4 projection = m_camera.getProjectionMatrix(m_GUI.getViewportWidth(), m_GUI.getViewportHeight());

		
		m_renderer.setMat4("model", model);
		m_renderer.setMat4("view", view);
		m_renderer.setMat4("projection", projection);
		m_renderer.setVec4("color", m_sphere.color);

		m_renderer.renderSphere(m_sphere);
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
			m_camera.mouseLook(xoffset, yoffset, true);
		}
		
			
	}

	void processScroll(double yoffset) {
		m_camera.scrollZoom(static_cast<float>(yoffset));
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