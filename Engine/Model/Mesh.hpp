#pragma once

#include <vector>

#include <glm/glm.hpp>

struct Material {
	glm::vec3 color;
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO = 0, VBO = 0, EBO = 0;
};

