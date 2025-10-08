#include "ForestScene.h"

#include "ShaderProgram.h"
#include "Model.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"

#include "bushes.h"
#include "gift.h"
#include "suzi_smooth.h"
#include "tree.h"

#include <cstdlib>
#include <ctime>

static const char* fs_color = R"(#version 330 core
in vec3 vColor;

out vec4 fragColor;
void main(){
    fragColor = vec4(vColor, 1.0);
})";

static const char* vs_color_matrix = R"(#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;

uniform mat4 modelMatrix; 

out vec3 vColor;
void main(){
    vColor = aColor;
    gl_Position = modelMatrix * vec4(aPos, 1.0);
})";

static const char* fs_uniform_color = R"(#version 330 core
    uniform vec3 objectColor; // Uniform proměnná pro barvu
    out vec4 fragColor;
    void main(){
        fragColor = vec4(objectColor, 1.0);
    })";

void ForestScene::setup() {
    srand(time(NULL));

    ShaderProgram* progColor = makeProgram(vs_color_matrix, fs_color);
    ShaderProgram* progUniform = makeProgram(vs_color_matrix, fs_uniform_color);

    Model* suziModel = makeModel(suziSmooth, sizeof(suziSmooth), 6 * sizeof(float));
    suziModel->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    suziModel->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (3 * sizeof(float)));

    Model* treeModel = makeModel(tree, sizeof(tree), 6 * sizeof(float));
    treeModel->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    treeModel->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 3 * sizeof(float));

    Model* bushModel = makeModel(bushes, sizeof(bushes), 6 * sizeof(float));
    bushModel->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    bushModel->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 3 * sizeof(float));

    Model* giftModel = makeModel(gift, sizeof(gift), 6 * sizeof(float));
    giftModel->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    giftModel->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (3 * sizeof(float)));

    // SUZI
    DrawableObject* suzi = addDrawable(suziModel, progColor, GL_TRIANGLES, suziModel->getVertexCount());
    auto& suziTransform = suzi->getTransform();
    auto suziRotation = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    suziRotation_ = suziRotation.get();
   
    suziTransform.add(std::make_unique<Scale>(glm::vec3(0.2f)));
    suziTransform.add(std::move(suziRotation));
    suziTransform.add(std::make_unique<Translate>(glm::vec3(0.0f, 0.0f, 0.0f)));


    for (int i = 0; i < 30; ++i) {
        float x = (rand() / (float)RAND_MAX) * 1.8f - 0.9f;
        float y = (rand() / (float)RAND_MAX) * 1.3f - 1.0f;
        float z = 1.0f;

        float scl = 0.2f + (rand() / (float)RAND_MAX) * 0.3f;

        float rot = (rand() / (float)RAND_MAX) * 360.0f;

        int modelType = rand() % 3;

        if (modelType == 0) { // TREE
            DrawableObject* treeObj = addDrawable(treeModel, progColor, GL_TRIANGLES, treeModel->getVertexCount());
            auto& t = treeObj->getTransform();

            float treeScale = scl * 0.2f;

            t.add(std::make_unique<Scale>(glm::vec3(treeScale)));
            t.add(std::make_unique<Translate>(glm::vec3(x, y, z)));
        }
        else if (modelType == 1) { // BUSH
            DrawableObject* bushObj = addDrawable(bushModel, progColor, GL_TRIANGLES, bushModel->getVertexCount());
            auto& t = bushObj->getTransform();
            t.add(std::make_unique<Scale>(glm::vec3(scl)));
            t.add(std::make_unique<Translate>(glm::vec3(x, y, z)));
        }
        else { // GIFT
            DrawableObject* randomGift = addDrawable(giftModel, progUniform, GL_TRIANGLES, giftModel->getVertexCount());
            auto& t = randomGift->getTransform();
            t.add(std::make_unique<Scale>(glm::vec3(scl)));
            t.add(std::make_unique<Rotate>(rot, glm::vec3(0.0f, 0.0f, 1.0f)));
            t.add(std::make_unique<Translate>(glm::vec3(x, y, z)));
        }
    }

    progUniform->useProgram();
    progUniform->setUniform("objectColor", glm::vec3(0.6f, 0.4f, 0.1f));
}

void ForestScene::update(float time) {
    if (suziRotation_) {
        suziRotation_->setAngle(time * 40.0f);
    }
}