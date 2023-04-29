#pragma once

#include "Camera.hpp"
#include "Star.hpp"
#include "Model/Generators.hpp"
#include "Model/Model.hpp"

class Terrain : public Model {
public:

	Terrain(float radius) : m_acceleration(9.8f), m_radius(radius) {
		
		Generators::generateSphere(128, 256, *m_mesh);
		
		translate(glm::vec3(0.0f, -1001.0f, 0.0f));
		scale(glm::vec3(radius));

		m_program.attachShader(VERTEX, "Shaders/Terrain.vert");
		m_program.attachShader(FRAGMENT, "Shaders/Terrain.frag");
		m_program.link();
		m_material = MaterialPresets::GOLD;
	}

	void update(const Camera& cam, const Star& star) {

		m_program.bind();
		m_program.setUniformMat4("u_model", m_modelMatrix);
		m_program.setUniformMat4("u_view", cam.getViewMatrix());
		m_program.setUniformMat4("u_projection", cam.getProjectionMatrix());

		m_program.setUniformVec4("light.color", star.getLight().color);
		m_program.setUniformVec3("light.direction", star.getLight().direction);

		m_program.setUniformVec4("mat.ambient", m_material.ambient);
		m_program.setUniformVec4("mat.diffuse", m_material.diffuse);
		m_program.setUniformVec4("mat.specular", m_material.specular);
		m_program.setUniformFloat("mat.shine", m_material.shine);

		m_program.setUniformVec3("u_cameraPos", cam.getPosition());
		m_program.setUniformFloat("u_globalAmbient", 0.1f);
		m_program.unbind();

	}

private:

	float m_radius;
	float m_acceleration; //g 9.8f

};