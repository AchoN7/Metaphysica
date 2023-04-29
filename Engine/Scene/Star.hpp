#pragma once

#include "Graphics/Light.hpp"
#include "Scene/Camera.hpp"
#include "Model/Generators.hpp"
#include "Model/Model.hpp"

class Star : public Model {
public:

	Star(float tilt) {
	
		float x = 0.0f;
		float y = glm::sin(glm::radians(90.0f - tilt));
		float z = -glm::cos(glm::radians(90.0f - tilt));
		m_offsetFromCamera = glm::vec3(x, y, z);

		float X = 0.0f;
		float Y = glm::sin(glm::radians(-tilt));
		float Z = -glm::cos(glm::radians(-tilt));
		m_axisOfRotation = glm::vec3(X,Y,Z);

		Generators::generateSphere(64, 128, *m_mesh);
		translate(m_offsetFromCamera);
		scale(glm::vec3(0.015f));
		m_material = MaterialPresets::STAR;
		m_light.color = glm::vec4(1.0f, 1.0f, 0.9f, 1.0f);
		m_light.intensity = 1.0f;
		m_light.direction = -m_position;

		m_program.attachShader(VERTEX, "Shaders/Star.vert");
		m_program.attachShader(FRAGMENT, "Shaders/Star.frag");
		m_program.link();
	}

	void update(const Camera& cam) {
		//m_offsetFromCamera = glm::rotate(m_offsetFromCamera, glm::radians(deltaTime * 10), m_axisOfRotation);

		translate(cam.getPosition() + m_offsetFromCamera);
		m_light.direction = -m_offsetFromCamera;

		m_program.bind();
		m_program.setUniformMat4("u_model", m_modelMatrix);
		m_program.setUniformMat4("u_view", cam.getViewMatrix());
		m_program.setUniformMat4("u_projection", cam.getProjectionMatrix());
		m_program.setUniformVec4("u_starColor", m_light.color);
		m_program.unbind();
	}

	auto getAxisOfRotation() const { return m_axisOfRotation; }
	auto getOffsetFromCamera() const { return m_offsetFromCamera; }
	const DirectionalLight& getLight() const { return m_light; }

	void setOffsetFromCamera(glm::vec3 newOffset) { m_offsetFromCamera = newOffset; }

private:

	DirectionalLight m_light;
	glm::vec3 m_offsetFromCamera;
	glm::vec3 m_axisOfRotation;

};