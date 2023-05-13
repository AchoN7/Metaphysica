#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light {
public:

	Light(glm::vec3 position, glm::vec3 direction, glm::vec4 color, float intensity);

	glm::vec3 getPosition() const { return m_position; }
	glm::vec3 getDirection() const { return m_direction; }
	glm::vec4 getColor() const { return m_color; }
	float getIntensity() const { return m_intensity; }
	glm::mat4 getLightSpaceMatrix() const { return m_lightSpaceMatrix; }

	void setColor(glm::vec4 color) { m_color = color; }
	void setIntensity(float intensity) {  m_intensity = intensity; }
	void setPosition(glm::vec3 position);
	void setDirection(glm::vec3 direction);

private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec4 m_color;
	float m_intensity;

	glm::mat4 m_lightSpaceMatrix;

	void calcLightSpaceMatrix();
};

