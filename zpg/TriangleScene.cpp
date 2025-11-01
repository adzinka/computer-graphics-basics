#include "TriangleScene.h"
#include "ShaderProgram.h" 
#include "Model.h"
#include "Rotate.h"
#include "Translate.h"
#include "ResourceManager.h"

void TriangleScene::setup(Camera& camera, ResourceManager& manager) {
    ShaderProgram* prog = manager.getShader("constant");
    Model* triModel = manager.getModel("triangle");

    DrawableObject* triObj = addDrawable(triModel, prog, GL_TRIANGLES, triModel->getVertexCount());
    triObj->setColor(glm::vec3(0.3f, 0.6f, 0.2f));
    auto transform = std::make_unique<CompositeTransform>();
    auto rotation = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)); 

    triangleRotation_ = rotation.get();
    transform->add(std::move(rotation));

    triObj->setTransform(std::move(transform));
}

void TriangleScene::update(float time, Camera& camera) {
    if (triangleRotation_) {
        triangleRotation_->setAngle(time * 80.0f);
    }
}