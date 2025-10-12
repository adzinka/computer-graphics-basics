#include "TriangleScene.h"
#include "ShaderProgram.h" 
#include "Model.h"
#include "Rotate.h"
#include "Translate.h"

static float triangle_vertices[] = {    
   0.0f, 0.6f, 0.0f,      1.0f, 0.0f, 0.0f, 
   -0.52f, -0.3f, 0.0f,   0.0f, 1.0f, 0.0f, 
   0.52f, -0.3f, 0.0f,    0.0f, 0.0f, 1.0f 
};

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

void TriangleScene::setup(Camera& camera) {
    ShaderProgram* prog = makeProgram(vs_color_matrix, fs_color, camera);

    Model* triModel = makeModel(triangle_vertices, sizeof(triangle_vertices), 6 * sizeof(float));
    triModel->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    triModel->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 3 * sizeof(float));

    DrawableObject* triObj = addDrawable(triModel, prog, GL_TRIANGLES, triModel->getVertexCount());

    auto& transform = triObj->getTransform();
    auto rotation = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)); 

    triangleRotation_ = rotation.get();
    transform.add(std::move(rotation));
}

void TriangleScene::update(float time) {
    if (triangleRotation_) {
        triangleRotation_->setAngle(time * 80.0f);
    }
}