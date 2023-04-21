#pragma once

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Mesh.hpp"

class Model {
public:
    Model(glm::vec3 pos = glm::vec3(0.0f), float scale = 1.0f) :
        m_position(pos),
        m_orientation(glm::vec3(0.0f, 0.0f, 1.0f)),
        m_angle(0.0f),
        m_scale(scale),
        m_modelMatrix(glm::mat4(1.0f)),
        m_material(),
        m_mesh()
    {}

    void setMesh(std::shared_ptr<Mesh> mesh) {
        m_mesh = mesh;
    }

    void translate(glm::vec3 newPosition) {
        m_position = newPosition;
        updateModelMatrix();
    }

    void rotate(float angleDegrees, glm::vec3 axis) {
        m_orientation = axis;
        m_angle = glm::radians(angleDegrees);
        updateModelMatrix();
    }

    void scale(float newScale) {
        m_scale = glm::vec3(newScale);
        updateModelMatrix();
    }

    glm::vec3 getPosition() const { return m_position; }
    glm::vec3 getScale() const { return m_scale; }
    glm::mat4 getModelMatrix() const { return m_modelMatrix; }
    Material& getMaterial() { return m_material; }
    const auto& getMesh() const { return m_mesh; }

private:

    glm::vec3 m_position;
    glm::vec3 m_orientation;
    float m_angle;
    glm::vec3 m_scale;
    glm::mat4 m_modelMatrix;

    Material m_material;
    std::shared_ptr<Mesh> m_mesh;

    void updateModelMatrix() {
        m_modelMatrix = glm::translate(glm::mat4(1.0f), m_position);
        m_modelMatrix = glm::rotate(m_modelMatrix, m_angle, m_orientation);
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
    }

};
