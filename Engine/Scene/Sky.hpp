#pragma once

#include "Graphics/Light.hpp"
#include "Camera.hpp"
#include "Model/Generators.hpp"
#include "Model/Model.hpp"

class Sky : public Model {
public:

	Sky() : m_skyColor((0.529f, 0.811f, 0.922f)) {
		m_program.attachShader(VERTEX, "Shaders/Sky.vert");
		m_program.attachShader(FRAGMENT, "Shaders/Sky.frag");
		m_program.link();

		Generators::generateSphere(64, 128, *m_mesh);
	}

	void update(const Camera& cam, glm::vec3 sunOffset) {
		translate(cam.getPosition());
		static const glm::vec3 initialSunPos = sunOffset;
		float timeOfDayFactor = glm::dot(glm::normalize(sunOffset), initialSunPos);
		m_program.bind();
		m_program.setUniformMat4("u_model", m_modelMatrix);
		m_program.setUniformMat4("u_view", cam.getViewMatrix());
		m_program.setUniformMat4("u_projection", cam.getProjectionMatrix());
		m_program.setUniformFloat("u_cycle", timeOfDayFactor);
		m_program.unbind();

	}

private:

	glm::vec3 m_skyColor;

};