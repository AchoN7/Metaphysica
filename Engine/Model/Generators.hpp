#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Mesh.hpp"

namespace Generators {

    static void generateSphere(unsigned int stacks, unsigned int sections, Mesh& mesh)  {
        mesh.vertices.clear();
        mesh.indices.clear();

        constexpr float PI = 3.14159265f;
        for (unsigned int i = 0; i <= stacks; ++i) {
            
            float v = i / (float)stacks;
            float theta = v * PI;

            for (unsigned int j = 0; j <= sections; ++j) {
                float u = j / (float)sections;
                float phi = u * 2 * PI;

                float x = cos(phi) * sin(theta);
                float y = cos(theta);
                float z = sin(phi) * sin(theta);

                Vertex v;
                v.position = glm::vec3(x, y, z);
                v.normal = glm::normalize(v.position);
                mesh.vertices.push_back(v);
            }
        }

        for (unsigned int i = 0; i < stacks; ++i) {
            for (unsigned int j = 0; j < sections; ++j) {
                int a = i * (sections + 1) + j;
                int b = a + sections + 1;

                mesh.indices.push_back(a);
                mesh.indices.push_back(b);
                mesh.indices.push_back(a + 1);
                            
                mesh.indices.push_back(b);
                mesh.indices.push_back(b + 1);
                mesh.indices.push_back(a + 1);
            }
        }
    }

    static void generateCircle(unsigned int sections, Mesh& mesh) {
        mesh.vertices.clear();
        mesh.indices.clear();

        constexpr float PI = 3.14159265f;
        const float angleStep = 2.0f * PI / (float)sections;

        Vertex v0;
        v0.position = glm::vec3(0.0f);
        v0.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        mesh.vertices.push_back(v0);

        for (unsigned int i = 0; i < sections; ++i) {
            float angle = angleStep * i;
            float x = cos(angle);
            float y = sin(angle);

            Vertex v;
            v.position = glm::vec3(x, y, 0.0f);
            v.normal = glm::normalize(glm::cross(v.position, glm::vec3(-y, x, 0.0f)));
            mesh.vertices.push_back(v);
        }

        for (unsigned int i = 1; i <= sections; ++i) {
            mesh.indices.push_back(0);
            mesh.indices.push_back(i);
            mesh.indices.push_back(i == sections ? 1 : i + 1);
        }
    }

    static void generateSquare(Mesh& mesh) {
        mesh.vertices.clear();
        mesh.indices.clear();

        const float halfSideLength = 0.5f;
        const glm::vec3 vertices[] = {
            glm::vec3(-halfSideLength, halfSideLength, 0.0f),
            glm::vec3(halfSideLength, halfSideLength, 0.0f),
            glm::vec3(halfSideLength, -halfSideLength, 0.0f),
            glm::vec3(-halfSideLength, -halfSideLength, 0.0f)
        };
        const glm::vec3 normals[] = {
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        };
        const unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        for (int i = 0; i < 4; ++i) {
            Vertex v;
            v.position = vertices[i];
            v.normal = normals[i];
            mesh.vertices.push_back(v);
        }

        for (int i = 0; i < 6; ++i) {
            mesh.indices.push_back(indices[i]);
        }
    }

}