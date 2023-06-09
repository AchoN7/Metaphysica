#include "Engine.hpp"

Engine::Engine() : m_window(), m_renderer(), m_activeScene(), m_GUI(m_window), m_deltaTime(0.0f), m_lastFrame(0.0f)
{
	glfwSetWindowUserPointer(m_window.getWindow(), this);
	glfwSetWindowSizeCallback(m_window.getWindow(), Engine::windowSizeCallback);
	//glfwSetCursorPosCallback(m_window.getWindow(), Engine::cursorCallback);
	glfwSetScrollCallback(m_window.getWindow(), Engine::scrollCallback);

	m_viewport = std::make_unique<Viewport>(m_renderer);
	m_editor = std::make_unique<Editor>(m_activeScene);
	m_settings = std::make_unique<Settings>(m_activeScene, m_renderer);
	m_console = std::make_unique<Console>();
	m_shapes = std::make_unique<Shapes>();
	m_metrics = std::make_unique<Metrics>();

	m_renderer.recreateFramebuffer(m_viewport->getWidth(), m_viewport->getHeight());
	m_activeScene.getCamera().onViewportResized(m_viewport->getWidth(), m_viewport->getHeight());

	for (auto& model : m_activeScene.getModels()) {
		m_renderer.bindModel(*model);
	}

	m_GUI.addGUIelement(m_viewport.get());
	m_GUI.addGUIelement(m_editor.get());
	m_GUI.addGUIelement(m_settings.get());
	m_GUI.addGUIelement(m_console.get());
	m_GUI.addGUIelement(m_shapes.get());
	m_GUI.addGUIelement(m_metrics.get());
}

Engine::~Engine() {}

void Engine::mainLoop() {

	while (!glfwWindowShouldClose(m_window.getWindow())) {

		float currentFrame = static_cast<float>(glfwGetTime());
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		inputs();
		update();
		render();

	}
}

void Engine::inputs() {
	//TODO: encapsulate in own class?
	//TODO: make sure to move if the mouse is on the viewport
	glfwPollEvents();

	if (glfwGetKey(m_window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
		m_activeScene.getCamera().move(Camera::Direction::FORWARD, m_deltaTime);
	if (glfwGetKey(m_window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		m_activeScene.getCamera().move(Camera::Direction::BACKWARD, m_deltaTime);
	if (glfwGetKey(m_window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		m_activeScene.getCamera().move(Camera::Direction::LEFT, m_deltaTime);
	if (glfwGetKey(m_window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		m_activeScene.getCamera().move(Camera::Direction::RIGHT, m_deltaTime);
}

void Engine::update() {
	auto& viewportRef = *m_viewport;
	Camera& sceneCameraRef = m_activeScene.getCamera();

	if (viewportRef.isResized()) {
		m_renderer.recreateFramebuffer(viewportRef.getWidth(), viewportRef.getHeight());
		sceneCameraRef.onViewportResized(viewportRef.getWidth(), viewportRef.getHeight());
		viewportRef.clearResizeFlag();
	}

	//TEMP
	if (viewportRef.isFocused() && viewportRef.isHovered()) {
		double xpos, ypos;
		glfwGetCursorPos(m_window.getWindow(), &xpos, &ypos);
		processCursor(xpos, ypos);
	}

	m_activeScene.update(m_deltaTime);
	m_viewport->update();
}

void Engine::render() {
	m_renderer.renderScene(m_activeScene);
	m_GUI.display();
	m_window.swapBuffers();
}

void Engine::updateDimensions(int windowWidth, int windowHeight) {
	m_window.updateDimensions(windowWidth, windowHeight);
	m_GUI.updateDimensions(windowWidth, windowHeight);

}

void Engine::processCursor(double xposIn, double yposIn) {
	auto& viewportRef = *m_viewport;

	static bool firstMouse = true;
	static float lastX = static_cast<float>(viewportRef.getWidth() / 2.0f);
	static float lastY = static_cast<float>(viewportRef.getHeight() / 2.0f);

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

	if (viewportRef.isHovered() && viewportRef.isRightClicked()) {
		m_activeScene.getCamera().mouseLook(xoffset, yoffset, true);
	}


}

void Engine::processScroll(double yoffset) {
	m_activeScene.getCamera().scrollZoom(static_cast<float>(yoffset));
}

//////////GLFW CALLBACKS
void Engine::windowSizeCallback(GLFWwindow* window, int width, int height) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->updateDimensions(width, height);

	//single update/render sequence to update screen contents while resizing
	engine->update();
	engine->render();
}

void Engine::cursorCallback(GLFWwindow* window, double xposIn, double yposIn) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->processCursor(xposIn, yposIn);
}

void Engine::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->processScroll(yoffset);
}

	