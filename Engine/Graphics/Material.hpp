#pragma once

#include <glm/glm.hpp>

struct Material {
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shine;
};

class MaterialPresets {
public:
	static const Material GOLD;
	static const Material JADE;
	static const Material PEARL;
	static const Material SILVER;
	static const Material LIMESTONE;

	static const Material STAR;
};
