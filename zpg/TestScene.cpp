#include "TestScene.h"
#include "ResourceManager.h"
#include "Translate.h"
#include "Scale.h"
#include "Light.h" 

void TestScene::setup(Camera& camera, ResourceManager& manager) {

    addLight(std::make_unique<Light>(
        glm::vec3(0.0f, 3.0f, -5.0f), 
        glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), 
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
        1.0f, 0.0f, 0.0f 
    ));

    ShaderProgram* progBad = manager.getShader("bad_phong");
    ShaderProgram* progGood = manager.getShader("phong");
    Model* sphereModel = manager.getModel("sphere");

    glm::vec3 color = glm::vec3(0.5f); 

    DrawableObject* badSphere = addDrawable(sphereModel, progBad, GL_TRIANGLES, sphereModel->getVertexCount());
    badSphere->setColor(color);
    auto badTransform = std::make_unique<CompositeTransform>();
    badTransform->add(std::make_unique<Scale>(glm::vec3(0.5f)));
    badTransform->add(std::make_unique<Translate>(glm::vec3(-0.7f, 0.0f, 0.0f))); 
    badSphere->setTransform(std::move(badTransform));

    DrawableObject* goodSphere = addDrawable(sphereModel, progGood, GL_TRIANGLES, sphereModel->getVertexCount());
    goodSphere->setColor(color);
    auto goodTransform = std::make_unique<CompositeTransform>();
    goodTransform->add(std::make_unique<Scale>(glm::vec3(0.5f)));
    goodTransform->add(std::make_unique<Translate>(glm::vec3(0.7f, 0.0f, 0.0f))); 
    goodSphere->setTransform(std::move(goodTransform));
}

void TestScene::update(float time, Camera& camera) {
    updateSceneLights();
}