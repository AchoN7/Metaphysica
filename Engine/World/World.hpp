#pragma once

#include <vector>

#include "Graphics/Light.hpp"
#include "Model/Model.hpp"
#include "Model/Generators.hpp"

class World {
public:

	World() : m_starLight(), m_camera(), m_ground(), m_star()
	{

		//create ground
		std::shared_ptr<Mesh> groundMesh = std::make_shared<Mesh>();
		Generators::generateSquare(*groundMesh);

		m_ground.setMesh(groundMesh);
		m_ground.rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		m_ground.scale(100.0f);
		m_ground.getMaterial().color = glm::vec3(0.2f, 0.9f, 0.2f);

		//set camera pos relative to ground
		m_camera.setPosition(glm::vec3(0.0f, m_ground.getPosition().y + 1.0f, 0.0f));
		
		//create star
		std::shared_ptr<Mesh> starMesh = std::make_shared<Mesh>();
		Generators::generateSphere(64, 128, *starMesh);

		m_star.setMesh(starMesh);
		glm::vec3 sunPos = m_camera.getPosition() + glm::vec3(0.0f, 0.3f, -0.9f);
		m_star.translate(sunPos);
		m_star.scale(0.03f);
		m_star.getMaterial().color = glm::vec3(1.0f);
		
		//m_starLight.position = m_star.getPosition();
		m_starLight.direction = -glm::normalize(sunPos - m_camera.getPosition());
		m_starLight.color = glm::vec3(1.0f);

	}

	void update(float deltaTime) {
		glm::vec3 sunPos = m_camera.getPosition() + glm::vec3(0.0f, 0.3f, -0.9f);
		m_star.translate(sunPos);
		m_starLight.direction = -glm::normalize(sunPos - m_camera.getPosition());
	}

	void addModel(/* asset ID */) {
		//m_models.emplace_back();
	}

	auto& getStar() { return m_star; }
	auto& getStarLight() { return m_starLight; }
	auto& getGround() { return m_ground; }
	auto& getCamera() { return m_camera; }
	auto& getModels() { return m_models; }

	void setStarLightPos(glm::vec3 pos) { m_starLight.position = pos; }
	void setStarLightCol(glm::vec3 col) { m_starLight.position = col; }

private:

	Model m_star;
	Model m_ground;
	Light m_starLight;
	//Model m_moon;
	//Sky m_skysphere;
	
	Camera m_camera;

	std::vector<Model> m_models;


};