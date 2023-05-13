#include "Model.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

Model::Model(glm::vec3 pos, float scale) :
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

void Model::setMesh(std::shared_ptr<Mesh> mesh) {
    m_mesh = mesh;
}

void Model::setMaterial(Material mat) {
    m_material = mat;
}

void Model::translate(glm::vec3 newPosition) {
    m_position = newPosition;
    updateModelMatrix();
}

void Model::rotate(float angleDegrees, glm::vec3 axis) {
    m_orientation = axis;
    m_angle = glm::radians(angleDegrees);
    updateModelMatrix();
}

void Model::scale(glm::vec3 scale = glm::vec3(1.0f)) {
    m_scale = scale;
    updateModelMatrix();
}

void Model::updateModelMatrix() {
    m_modelMatrix = glm::translate(glm::mat4(1.0f), m_position);
    m_modelMatrix = glm::rotate(m_modelMatrix, m_angle, m_orientation);
    m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
}