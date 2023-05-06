#pragma once

#include "Sky.hpp"
#include "Camera.hpp"
#include "Physics/Physics.hpp"

class Scene {
public:

	Scene();

	Camera& getCamera() { return m_camera; }
	Physics& getPhysics() { return m_physics; }
	std::vector<std::shared_ptr<Model>>& getModels() { return m_models; }
	std::vector<std::shared_ptr<Light>>& getLights() { return m_lights; }

	void update(float deltaTime);

	void pausePhysics() { m_physics.pause(); }
	void resumePhysics() { m_physics.resume(); }
	void speedupTime() { m_physics.speedup(); }
	void normalTime() { m_physics.normal(); }

private:

	Camera m_camera;
	Physics m_physics;

	std::vector<std::shared_ptr<Model>> m_models;
	std::vector<std::shared_ptr<Light>> m_lights;

};