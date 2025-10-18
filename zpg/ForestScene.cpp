#include "ForestScene.h"

#include "ShaderProgram.h"
#include "Model.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "ResourceManager.h"

#include <cstdlib>
#include <ctime>

void ForestScene::setup(Camera& camera, ResourceManager& manager) {
    srand(time(NULL));

    ShaderProgram* progColor = manager.getShader("color_shader");
    ShaderProgram* progUniform = manager.getShader("uniform_color_shader");
    ShaderProgram* lightning = manager.getShader("lightning");


    Model* suziModel = manager.getModel("suzi");
    Model* giftModel = manager.getModel("gift");
    Model* treeModel = manager.getModel("tree");
    Model* bushModel = manager.getModel("bush");
    Model* plainModel = manager.getModel("plain");

    DrawableObject* ground = addDrawable(plainModel, progColor, GL_TRIANGLES, plainModel->getVertexCount());
    auto& groundTransform = ground->getTransform();

    groundTransform.add(std::make_unique<Scale>(glm::vec3(25.0f, 1.0f, 25.0f)));
    groundTransform.add(std::make_unique<Translate>(glm::vec3(0.0f, 0.0f, 0.0f)));

    int objectCount = 100; 
    for (int i = 0; i < objectCount; ++i) {
  
        float x = (rand() / (float)RAND_MAX) * 50.0f - 25.0f;
        float z = (rand() / (float)RAND_MAX) * 50.0f - 25.0f;

        float scl = 0.8f + (rand() / (float)RAND_MAX) * 0.7f; 
        float rot = (rand() / (float)RAND_MAX) * 360.0f;

        if (i < 50) { 
            DrawableObject* treeObj = addDrawable(treeModel, lightning, GL_TRIANGLES, treeModel->getVertexCount());
            auto& t = treeObj->getTransform();
            t.add(std::make_unique<Scale>(glm::vec3(scl)));
            t.add(std::make_unique<Rotate>(rot, glm::vec3(0.0f, 1.0f, 0.0f)));
            t.add(std::make_unique<Translate>(glm::vec3(x, 0.0f, z)));
        }
        else { 
            DrawableObject* bushObj = addDrawable(bushModel, lightning, GL_TRIANGLES, bushModel->getVertexCount());
            auto& t = bushObj->getTransform();
            t.add(std::make_unique<Scale>(glm::vec3(scl)));
            t.add(std::make_unique<Rotate>(rot, glm::vec3(0.0f, 1.0f, 0.0f)));
            t.add(std::make_unique<Translate>(glm::vec3(x, 0.0f, z)));
        }
    }


    for (int i = 0; i < 10; ++i) {
        float x = (rand() / (float)RAND_MAX) * 50.0f - 25.0f;
        float z = (rand() / (float)RAND_MAX) * 50.0f - 25.0f;
        DrawableObject* randomGift = addDrawable(giftModel, progUniform, GL_TRIANGLES, giftModel->getVertexCount());
        auto& t = randomGift->getTransform();
        t.add(std::make_unique<Scale>(glm::vec3(3.0f)));
        t.add(std::make_unique<Translate>(glm::vec3(x, 0.0f, z)));
    }

    progUniform->useProgram();
    progUniform->setUniform("objectColor", glm::vec3(0.6f, 0.4f, 0.1f));
}

void ForestScene::update(float time) {
    if (suziRotation_) {
        suziRotation_->setAngle(time * 40.0f);
    }
}