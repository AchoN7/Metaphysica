#pragma once

#include "Sky.hpp"
#include "Star.hpp"
#include "Terrain.hpp"
#include "Camera.hpp"
#include "Physics/Physics.hpp"

class Scene {
public:

	Scene() : m_camera(), m_star(23.5f), m_planet(1000.0f), m_sky() {
	
	}

	Sky& getSky() { return m_sky; }
	Star& getStar() { return m_star; }
	Terrain& getTerrain() { return m_planet; }
	Camera& getCamera() { return m_camera; }
	Physics& getPhysics() { return m_physics; }
	std::vector<Model>& getModels() { return m_models; }

	void update(float deltaTime) {
		//update physics
		//update uniforms
		m_physics.updateStar(m_star, deltaTime);

		m_star.update(m_camera);
		//change sky colors depending on the day/night cycle
		
		m_sky.update(m_camera, m_star.getOffsetFromCamera());
		m_planet.update(m_camera, m_star);
	}

	void pausePhysics() { m_physics.pause(); }
	void resumePhysics() { m_physics.resume(); }

private:

	Sky m_sky;
	Star m_star;
	Terrain m_planet;
	Camera m_camera;
	Physics m_physics;

	std::vector<Model> m_models;

};