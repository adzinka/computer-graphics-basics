#include "SpheresScene.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "CompositeTransform.h"

#include "ResourceManager.h"

void SpheresScene::setup(Camera& camera, ResourceManager& manager) {

    addLight(std::make_unique<Light>(
        glm::vec3(0.0f, 0.0f, 0.0f),                
        glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),         
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),         
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),       
        1.0f,  
        0.0f, 
        0.0f   
    ));

    ShaderProgram* progPhong = manager.getShader("phong");

    Model* sphereModel = manager.getModel("sphere");

    progPhong->useProgram();
 
    std::vector<glm::vec3> positions = {
            { 0.6f,  0.0f, 0.0f},
            {-0.6f,  0.0f, 0.0f},
            { 0.0f,  0.6f, 0.0f},
            { 0.0f, -0.6f, 0.0f}
    };

    for (int i = 0; i < 4; ++i) {

        DrawableObject* sphereObj = addDrawable(sphereModel, progPhong, GL_TRIANGLES, sphereModel->getVertexCount());
        sphereObj->setColor(glm::vec3(0.385f, 0.647f, 0.812f));

        auto transform = std::make_unique<CompositeTransform>();

        transform->add(std::make_unique<Scale>(glm::vec3(0.3f))); 
        transform->add(std::make_unique<Translate>(positions[i]));
        sphereObj->setTransform(std::move(transform));
    }
}

void SpheresScene::update(float time, Camera& camera) {
    updateSceneLights();
}