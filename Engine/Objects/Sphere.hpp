#pragma once

#include <vector>
#include <glm/glm.hpp>

enum SphereQuality : unsigned char {
    LOW = 8,
    MED = 16,
    HIGH = 32,
    ULTRA = 64
};

struct Sphere {

	Sphere(glm::vec4 c, SphereQuality quality ) {
		color = c;
        VBO = 0;
        EBO = 0;

        //horizontal slices, vertical slices
        regenerateData(quality, quality * 2);
        
	}

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indecies;

	glm::vec4 color;

	unsigned int VBO;
	unsigned int EBO;

    void regenerateData(unsigned char slices, unsigned char segments) {
        vertices.clear(); 
        indecies.clear();

        // Generate sphere vertex positions
        for (int i = 0; i <= slices; ++i) {
            float v = i / (float)slices;
            float theta = v * glm::pi<float>();

            for (int j = 0; j <= segments; ++j) {
                float u = j / (float)segments;
                float phi = u * 2 * glm::pi<float>();

                float x = cos(phi) * sin(theta);
                float y = cos(theta);
                float z = sin(phi) * sin(theta);

                vertices.push_back(glm::vec3(x, y, z));
            }
        }

        // Generate sphere index data
        for (int i = 0; i < slices; ++i) {
            for (int j = 0; j < segments; ++j) {
                int a = i * (segments + 1) + j;
                int b = a + segments + 1;

                indecies.push_back(a);
                indecies.push_back(b);
                indecies.push_back(a + 1);

                indecies.push_back(b);
                indecies.push_back(b + 1);
                indecies.push_back(a + 1);
            }
        }
    }
};