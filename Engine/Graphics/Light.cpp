#include "Light.hpp"

Light::Light(glm::vec3 position, glm::vec3 direction, glm::vec4 color, float intensity)
	: m_position(position), m_direction(direction), m_color(color), m_intensity(intensity)
{
	calcLightSpaceMatrix();
}

void Light::setPosition(glm::vec3 position) {
	m_position = position;
	m_direction = -m_position;
	calcLightSpaceMatrix();
}

void Light::setDirection(glm::vec3 direction) {
	m_direction = direction;
	m_position = -m_direction;
	calcLightSpaceMatrix();
}

void Light::calcLightSpaceMatrix() {
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 lightView = glm::lookAt(m_position, m_direction, upVector);

	float near_plane = 0.1f, far_plane = 100.0f;
	float frustum_size = 50.0f;
	glm::mat4 lightProjection = glm::ortho(-frustum_size, frustum_size, -frustum_size, frustum_size, near_plane, far_plane);

	m_lightSpaceMatrix = lightProjection * lightView;
}