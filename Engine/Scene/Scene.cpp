#pragma once

#include "Scene.hpp"
#include "Model/Generators.hpp"

Scene::Scene() {

	//ground 
	std::shared_ptr<Model> flatPlane;
	flatPlane.reset(new Model());
	std::shared_ptr<Mesh> cuboidMesh(new Mesh());
	Generators::generateCuboid(*cuboidMesh, 100, 1, 100);
	flatPlane->setMesh(cuboidMesh);
	Material matPlane(MaterialPresets::GOLD);
	flatPlane->setMaterial(matPlane);
	flatPlane->getProgram().attachShader(VERTEX, "Shaders/Model.vert");
	flatPlane->getProgram().attachShader(FRAGMENT, "Shaders/Model.frag");
	flatPlane->getProgram().link();
	flatPlane->translate(glm::vec3(0.0f, -1.5f, 0.0f));
	m_models.push_back(flatPlane);

	//sphere
	std::shared_ptr<Model> modelSphere;
	modelSphere.reset(new Model());
	std::shared_ptr<Mesh> sphereMesh(new Mesh());
	Generators::generateSphere(32, 64, *sphereMesh);
	modelSphere->setMesh(sphereMesh);
	Material matSphere(MaterialPresets::SILVER);
	modelSphere->setMaterial(matSphere);
	modelSphere->getProgram().attachShader(VERTEX, "Shaders/Model.vert");
	modelSphere->getProgram().attachShader(FRAGMENT, "Shaders/Model.frag");
	modelSphere->getProgram().link();
	modelSphere->translate(glm::vec3(0.0f, 0.0f, -3.0f));
	m_models.push_back(modelSphere);

	//directional light
	constexpr float rad = glm::radians(45.0f);
	float x = 20 * glm::cos(rad);
	float y = 20 * glm::sin(rad);
	float z = 20 * glm::cos(rad);
	glm::vec3 lightPos{ x, y, z };
	constexpr glm::vec4 lightColor{ 1.0f, 1.0f, 0.9f, 1.0f };
	float lightIntensity = 1.0f;
	m_light.reset(new Light(lightPos, -lightPos, lightColor, lightIntensity));
}

void Scene::update(float deltaTime) {

	for (auto& model : m_models) {
		auto& modelMat = model->getMaterial();
		auto& modelProgram = model->getProgram();

		modelProgram.bind();
		modelProgram.setUniformMat4("u_model", model->getModelMatrix());
		modelProgram.setUniformMat4("u_view", m_camera.getViewMatrix());
		modelProgram.setUniformMat4("u_projection", m_camera.getProjectionMatrix());
		modelProgram.setUniformMat4("u_lightSpaceMatrix", m_light->getLightSpaceMatrix());

		modelProgram.setUniformVec4("light.color", m_light->getColor());
		modelProgram.setUniformVec3("light.direction", m_light->getDirection());
		//modelProgram.setUniformVec3("light.position", mainLight->getPosition());

		modelProgram.setUniformVec4("mat.ambient", modelMat.ambient);
		modelProgram.setUniformVec4("mat.diffuse", modelMat.diffuse);
		modelProgram.setUniformVec4("mat.specular", modelMat.specular);
		modelProgram.setUniformVec4("mat.emissive", glm::vec4(0));
		modelProgram.setUniformFloat("mat.shine", modelMat.shine);

		modelProgram.setUniformVec3("u_cameraPos", m_camera.getPosition());
		modelProgram.setUniformFloat("u_globalAmbient", 0.7f);
		modelProgram.unbind();
	}

}