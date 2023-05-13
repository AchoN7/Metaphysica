#include "Generators.hpp"

using namespace glm;

void Generators::generateCircle(unsigned int sections, Mesh& mesh) {
    mesh.vertices.clear();
    mesh.indices.clear();

    constexpr float PI = 3.14159265f;
    const float angleStep = 2.0f * PI / (float)sections;

    Vertex v0;
    v0.position = vec3(0.0f);
    v0.normal = vec3(0.0f, 0.0f, 1.0f);
    mesh.vertices.push_back(v0);

    for (unsigned int i = 0; i < sections; ++i) {
        float angle = angleStep * i;
        float x = cos(angle);
        float y = sin(angle);

        Vertex v;
        v.position = vec3(x, y, 0.0f);
        v.normal = normalize(cross(v.position, vec3(-y, x, 0.0f)));
        mesh.vertices.push_back(v);
    }

    for (unsigned int i = 1; i <= sections; ++i) {
        mesh.indices.push_back(0);
        mesh.indices.push_back(i);
        mesh.indices.push_back(i == sections ? 1 : i + 1);
    }
}

void Generators::generateRectangle(Mesh& mesh, float length, float width) {
    const float halfLength = length / 2.0f;
    const float halfWidth = width / 2.0f;

    mesh.vertices.clear();
    mesh.indices.clear();

    const vec3 vertices[] = {
        vec3(-halfLength, halfWidth, 0.0f),
        vec3(halfLength, halfWidth, 0.0f),
        vec3(halfLength, -halfWidth, 0.0f),
        vec3(-halfLength, -halfWidth, 0.0f)
    };

    const vec3 normals[] = {
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f)
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

void Generators::generateSphere(unsigned int stacks, unsigned int sections, Mesh& mesh) {
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
            v.position = vec3(x, y, z);
            v.normal = normalize(v.position);
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

void Generators::generateCuboid(Mesh& mesh, unsigned int length, unsigned int height, unsigned int width) {
    mesh.vertices.clear();
    mesh.indices.clear();

    vec3 factor = vec3(length, height, width);
    vec3 vertices[8]{
        vec3(-0.5f, -0.5f, -0.5f) * factor,
        vec3(-0.5f, -0.5f, 0.5f)  * factor,
        vec3(-0.5f, 0.5f, -0.5f)  * factor,
        vec3(-0.5f, 0.5f, 0.5f)   * factor,
        vec3(0.5f, -0.5f, -0.5f)  * factor,
        vec3(0.5f, -0.5f, 0.5f)   * factor,
        vec3(0.5f, 0.5f, -0.5f)   * factor,
        vec3(0.5f, 0.5f, 0.5f)    * factor
    };

    static constexpr unsigned int indicesTemp[36] = {
      0, 1, 2,  2, 1, 3,  // Front face
      4, 5, 6,  6, 5, 7,  // Back face
      0, 4, 1,  1, 4, 5,  // Left face
      2, 3, 6,  6, 3, 7,  // Right face
      0, 2, 4,  4, 2, 6,  // Top face
      1, 5, 3,  3, 5, 7  // Bottom face
    };


    for (int i = 0; i < 36; i += 3) {
        Vertex v1, v2, v3;
        vec3 normal = normalize(cross(
                vertices[indicesTemp[i + 1]] - vertices[indicesTemp[i]], 
                vertices[indicesTemp[i + 2]] - vertices[indicesTemp[i]]
            ));

        v1.position = vertices[indicesTemp[i]];
        v2.position = vertices[indicesTemp[i + 1]];
        v3.position = vertices[indicesTemp[i + 2]];

        v1.normal = normal;
        v2.normal = normal;
        v3.normal = normal;

        mesh.vertices.push_back(v1);
        mesh.vertices.push_back(v2);
        mesh.vertices.push_back(v3);

        mesh.indices.push_back(static_cast<unsigned int>(mesh.vertices.size() - 3));
        mesh.indices.push_back(static_cast<unsigned int>(mesh.vertices.size() - 2));
        mesh.indices.push_back(static_cast<unsigned int>(mesh.vertices.size() - 1));
    }
}
