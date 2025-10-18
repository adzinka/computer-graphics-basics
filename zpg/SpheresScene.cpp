#include "SpheresScene.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"

#include "ResourceManager.h"

void SpheresScene::setup(Camera& camera, ResourceManager& manager) {
    
    ShaderProgram* prog = manager.getShader("color_shader");

    Model* sphereModel = manager.getModel("sphere");

    std::vector<glm::vec3> positions = {
        { 0.6f,  0.0f, 0.0f}, 
        {-0.6f,  0.0f, 0.0f}, 
        { 0.0f,  0.6f, 0.0f}, 
        { 0.0f, -0.6f, 0.0f} 
    };

    for (const auto& pos : positions) {
        
        DrawableObject* sphereObj = addDrawable(sphereModel, prog, GL_TRIANGLES, sphereModel->getVertexCount());

        auto& transform = sphereObj->getTransform();

        auto rotation = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        sphereRotations_.push_back(rotation.get());

        transform.add(std::make_unique<Scale>(glm::vec3(0.3f)));
        transform.add(std::move(rotation)); 
        transform.add(std::make_unique<Translate>(pos)); 
    }
}

void SpheresScene::update(float time) {
    int i = 0;
    for (auto* rotation : sphereRotations_) {
        float speed = 40.0f + i * 10.0f;
        rotation->setAngle(time * speed);
        i++;
    }
}