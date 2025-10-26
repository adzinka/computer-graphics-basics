#include "SpheresScene2.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "CompositeTransform.h"
#include "ResourceManager.h"
#include "Light.h" 

void SpheresScene2::setup(Camera& camera, ResourceManager& manager) {

   
    addLight(std::make_unique<Light>(
        glm::vec3(-2.0f, 2.0f, 2.0f),               
        glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),        
        glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),         
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),          
        1.0f,  
        0.0f,  
        0.0f   
    ));

    ShaderProgram* progConstant = manager.getShader("constant");
    ShaderProgram* progLambert = manager.getShader("lambert");
    ShaderProgram* progPhong = manager.getShader("phong");
    ShaderProgram* progBlinn = manager.getShader("blinn");

    Model* sphereModel = manager.getModel("sphere");

    std::vector<ShaderProgram*> shaders = { progConstant, progLambert, progPhong, progBlinn };

    std::vector<glm::vec3> colors = {
        { 1.0f, 0.0f, 0.0f }, 
        { 0.0f, 1.0f, 0.0f }, 
        { 0.0f, 0.0f, 1.0f }, 
        { 1.0f, 1.0f, 0.0f }  
    };

    std::vector<glm::vec3> positions = {
            { 0.6f,  0.0f, 0.0f},
            {-0.6f,  0.0f, 0.0f},
            { 0.0f,  0.6f, 0.0f},
            { 0.0f, -0.6f, 0.0f}
    };

    for (int i = 0; i < 4; ++i) {
        ShaderProgram* currentShader = shaders[i];

        DrawableObject* sphereObj = addDrawable(sphereModel, currentShader, GL_TRIANGLES, sphereModel->getVertexCount());

        sphereObj->setColor(colors[i]);

        auto transform = std::make_unique<CompositeTransform>();

        transform->add(std::make_unique<Scale>(glm::vec3(0.35f)));
        transform->add(std::make_unique<Translate>(positions[i]));
        sphereObj->setTransform(std::move(transform));
    }

}

void SpheresScene2::update(float time) {
    updateSceneLights();
}