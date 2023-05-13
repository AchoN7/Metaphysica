#pragma once

#include <memory>

#include "Graphics/Shader.hpp"
#include "Graphics/Material.hpp"
#include "Mesh.hpp"

class Model {
public:
    Model(glm::vec3 pos = glm::vec3(0.0f), float scale = 1.0f);

    void setMesh(std::shared_ptr<Mesh> mesh);
    void setMaterial(Material mat);
    void translate(glm::vec3 newPosition);
    void rotate(float angleDegrees, glm::vec3 axis);
    void scale(glm::vec3 scale);

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

    void updateModelMatrix();

};
