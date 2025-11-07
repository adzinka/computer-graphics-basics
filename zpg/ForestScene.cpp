#include "ForestScene.h"
#include "ResourceManager.h"

#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "CompositeTransform.h"
#include "Firefly.h"

#include <cstdlib>
#include <ctime>

ForestScene::ForestScene() {}
ForestScene::~ForestScene() {}

void ForestScene::setup(Camera& camera, ResourceManager& manager) {
    srand(time(NULL));

    ShaderProgram* progLambert = manager.getShader("lambert");
    ShaderProgram* progPhong = manager.getShader("phong");
    ShaderProgram* progConstant = manager.getShader("constant");

    Model* giftModel = manager.getModel("gift");
    Model* treeModel = manager.getModel("tree");
    Model* bushModel = manager.getModel("bush");
    Model* plainModel = manager.getModel("plain");
    Model* houseModel = manager.getModel("house");

    DrawableObject* ground = addDrawable(plainModel, progPhong, GL_TRIANGLES, plainModel->getVertexCount());
    ground->setColor(glm::vec3(0.2f, 0.6f, 0.2f));
   
    auto groundComposite = std::make_unique<CompositeTransform>();
    groundComposite->add(std::make_unique<Scale>(glm::vec3(25.0f, 1.0f, 25.0f)));
    groundComposite->add(std::make_unique<Translate>(glm::vec3(0.0f, 0.0f, 0.0f)));
    ground->setTransform(std::move(groundComposite));

    addLight(Light::createDirectionalLight(
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec4(0.05f, 0.05f, 0.08f, 1.0f),
        glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)
    ));

    auto flashlight = Light::createSpotlight(
        camera.getPosition(),
        camera.getFront(),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        Light::SPOTLIGHT_NARROW_INNER,
        Light::SPOTLIGHT_NARROW_OUTER,
        Light::ATTENUATION_MEDIUM,
        false 
    );

    flashlightLight_ = addLight(std::move(flashlight));

    int objectCount = 100;
    for (int i = 0; i < objectCount; ++i) {

        float x = (rand() / (float)RAND_MAX) * 50.0f - 25.0f;
        float z = (rand() / (float)RAND_MAX) * 50.0f - 25.0f;
        float scl = 0.8f + (rand() / (float)RAND_MAX) * 0.7f;
        float rot = (rand() / (float)RAND_MAX) * 360.0f;

        DrawableObject* obj = nullptr;

        if (i < 50) { 
            obj = addDrawable(treeModel, progPhong, GL_TRIANGLES, treeModel->getVertexCount());
            obj->setColor(glm::vec3(0.1f, 0.4f, 0.1f));
        }
        else {
            obj = addDrawable(bushModel, progPhong, GL_TRIANGLES, bushModel->getVertexCount());
            obj->setColor(glm::vec3(0.3f, 0.5f, 0.1f));
        }

        auto objComposite = std::make_unique<CompositeTransform>();
        objComposite->add(std::make_unique<Scale>(glm::vec3(scl)));
        objComposite->add(std::make_unique<Rotate>(rot, glm::vec3(0.0f, 1.0f, 0.0f)));
        objComposite->add(std::make_unique<Translate>(glm::vec3(x, 0.0f, z)));
        obj->setTransform(std::move(objComposite));
    }

    for (int i = 0; i < 10; ++i) {
        float x = (rand() / (float)RAND_MAX) * 50.0f - 25.0f;
        float z = (rand() / (float)RAND_MAX) * 50.0f - 25.0f;
        float rot = (rand() / (float)RAND_MAX) * 360.0f;

        DrawableObject* randomGift = addDrawable(giftModel, progConstant, GL_TRIANGLES, giftModel->getVertexCount());
        randomGift->setColor(glm::vec3(1.0f, 0.1f, 0.1f));

        auto giftComposite = std::make_unique<CompositeTransform>();
        giftComposite->add(std::make_unique<Scale>(glm::vec3(0.4f)));
        giftComposite->add(std::make_unique<Rotate>(rot, glm::vec3(0.0f, 1.0f, 0.0f)));
        giftComposite->add(std::make_unique<Translate>(glm::vec3(x, 0.0f, z)));
        randomGift->setTransform(std::move(giftComposite));
    }

    Model* sphereModel = manager.getModel("sphere");
    ShaderProgram* constantShader = manager.getShader("constant");
    int fireflyCount = 5; 

    for (int i = 0; i < fireflyCount; ++i) {
        float x = (rand() / (float)RAND_MAX) * 40.0f - 20.0f; 
        float y = (rand() / (float)RAND_MAX) * 3.0f + 0.5f;   
        float z = (rand() / (float)RAND_MAX) * 40.0f - 20.0f; 

        fireflies_.push_back(std::make_unique<Firefly>(
            this, sphereModel, constantShader, glm::vec3(x, y, z)
        ));
    }

    DrawableObject* house = addDrawable(
        houseModel,                  
        progPhong,                  
        GL_TRIANGLES,                
        houseModel->getVertexCount() 
    );

    house->setColor(glm::vec3(0.8f, 0.2f, 0.2f));

    auto houseTransform = std::make_unique<CompositeTransform>();
    houseTransform->add(std::make_unique<Scale>(glm::vec3(1.5f)));
    houseTransform->add(std::make_unique<Translate>(glm::vec3(0.0f, 0.0f, -5.0f)));
    house->setTransform(std::move(houseTransform));

}

void ForestScene::update(float time, Camera& camera) {

    for (auto& firefly : fireflies_) {
        firefly->update(time);
    }

    if (flashlightLight_) {
        flashlightLight_->setPosition(camera.getPosition());
        flashlightLight_->setDirection(camera.getFront());
    }

    updateSceneLights();

}