#pragma once

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Graphics/Shader.hpp"
#include "Graphics/Material.hpp"
#include "Mesh.hpp"

class Model {
public:
    Model(glm::vec3 pos = glm::vec3(0.0f), float scale = 1.0f) :
        m_position(pos),
        m_orientation(glm::vec3(0.0f, 0.0f, 1.0f)),
        m_angle(0.0f),
        m_scale(scale),
        m_modelMatrix(glm::mat4(1.0f)),
        m_program(),
        m_material(),
        m_mesh(std::make_shared<Mesh>()),
        m_forceMagnitude(0.0f),
        m_forceDirection(glm::vec3(0.0f))
    {
        updateModelMatrix();
    }

    void setMesh(std::shared_ptr<Mesh> mesh) {
        m_mesh = mesh;
    }

    void setMaterial(Material mat) {
        m_material = mat;
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

    void scale(glm::vec3 scale = glm::vec3(1.0f)) {
        m_scale = scale;
        updateModelMatrix();
    }

    glm::vec3 getPosition() const { return m_position; }
    glm::vec3 getScale() const { return m_scale; }
    glm::mat4 getModelMatrix() const { return m_modelMatrix; }
    Shader& getProgram() { return m_program; }
    const Material& getMaterial() const { return m_material; }
    const auto& getMesh() const { return m_mesh; }

protected:

    glm::vec3 m_position;
    glm::vec3 m_orientation;
    float m_angle;
    glm::vec3 m_scale;
    glm::mat4 m_modelMatrix;

    Shader m_program;
    Material m_material;
    std::shared_ptr<Mesh> m_mesh;

    float m_forceMagnitude;
    glm::vec3 m_forceDirection;

    void updateModelMatrix() {
        m_modelMatrix = glm::translate(glm::mat4(1.0f), m_position);
        m_modelMatrix = glm::rotate(m_modelMatrix, m_angle, m_orientation);
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
    }

};
