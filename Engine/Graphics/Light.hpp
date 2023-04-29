#pragma once

#include <glm/glm.hpp>

struct LightCreateInfo {
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
};

struct Light {

	glm::vec4 color;
	float intensity; //0-100
};

struct PositionalLight : public Light {
	glm::vec3 position;
};

struct DirectionalLight : public Light {
	glm::vec3 direction;
};
