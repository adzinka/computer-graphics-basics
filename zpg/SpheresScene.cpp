#include "SpheresScene.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "sphere.h"

static const char* fs_color = R"(#version 330 core
in vec3 vColor;

out vec4 fragColor;
void main(){
    fragColor = vec4(vColor, 1.0);
})";

static const char* vs_color_matrix = R"(#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix; 

out vec3 vColor;
void main(){
    vColor = aColor;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
})";

void SpheresScene::setup(Camera& camera) {
    
    ShaderProgram* prog = makeProgram(vs_color_matrix, fs_color, camera);
    Model* sphereModel = makeModel(sphere, sizeof(sphere), 6 * sizeof(float));
    sphereModel->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    sphereModel->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 3 * sizeof(float));


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