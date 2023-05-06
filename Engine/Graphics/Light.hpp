#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light {
public:

	Light(glm::vec3 position, glm::vec3 direction, glm::vec4 color, float intensity) 
		: m_position(position), m_direction(direction), m_color(color), m_intensity(intensity) 
	{
		calcLightSpaceMatrix();
	}

	glm::vec3 getPosition() const { return m_position; }
	glm::vec3 getDirection() const { return m_direction; }
	glm::vec4 getColor() const { return m_color; }
	float getIntensity() const { return m_intensity; }
	glm::mat4 getLightSpaceMatrix() const { return m_lightSpaceMatrix; }

	void setPosition(glm::vec3 position) { m_position = position; calcLightSpaceMatrix(); }
	void setDirection(glm::vec3 direction)  { m_direction = direction; calcLightSpaceMatrix(); }
	void setColor(glm::vec4 color) { m_color = color; }
	void setIntensity(float intensity) {  m_intensity = intensity; }

private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec4 m_color;
	float m_intensity;

	glm::mat4 m_lightSpaceMatrix;

	void calcLightSpaceMatrix() {
		glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 lightView = glm::lookAt(m_position, m_direction, upVector);

		float near_plane = 0.1f, far_plane = 100.0f;
		float frustum_size = 50.0f;
		glm::mat4 lightProjection = glm::ortho(-frustum_size, frustum_size, -frustum_size, frustum_size, near_plane, far_plane);

		m_lightSpaceMatrix = lightProjection * lightView;
	}
};

