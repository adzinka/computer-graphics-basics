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

    DrawableObject* ground = addDrawable(plainModel, progPhong, GL_TRIANGLES, plainModel->getVertexCount());
    ground->setColor(glm::vec3(0.2f, 0.6f, 0.2f));
   
    auto groundComposite = std::make_unique<CompositeTransform>();
    groundComposite->add(std::make_unique<Scale>(glm::vec3(25.0f, 1.0f, 25.0f)));
    groundComposite->add(std::make_unique<Translate>(glm::vec3(0.0f, 0.0f, 0.0f)));
    ground->setTransform(std::move(groundComposite));

    addLight(std::make_unique<Light>(
        glm::vec3(0.0f, 10.0f, 0.0f),       
        glm::vec4(0.05f, 0.05f, 0.08f, 1.0f), 
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  
        1.0f, 0.0f, 0.0,                    // Constant, Linear, Quadratic: Затухание 0, так как это общий свет
        LightType::Directional,                     // Тип
        glm::vec3(-0.2f, -1.0f, -0.3f),             // Направление света (сверху-слева-сзади)
        0.0f, 0.0f,
        true
    ));

    // --- 2. Spotlight (Фонарик) ---
    // Углы: 12.5 и 17.5 градусов. Переводим в косинус.
    float innerCutOff = glm::cos(glm::radians(12.5f));
    float outerCutOff = glm::cos(glm::radians(17.5f));

    auto flashlight = std::make_unique<Light>(
        camera.getPosition(),                       // Начальная позиция
        glm::vec4(0.0f),                            // Ambient 
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),          // Diffuse (яркий белый)
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),          // Specular 
        1.0f, 0.09f, 0.032f,                        // Attenuation (средняя дальность, ~20-30 единиц)
        LightType::Spot,                            // Тип
        camera.getFront(),                          // Начальное направление
        innerCutOff, outerCutOff,                   // Углы
        false                                       // Изначально ВЫКЛЮЧЕН
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
    int fireflyCount = 10; 

    for (int i = 0; i < fireflyCount; ++i) {
        float x = (rand() / (float)RAND_MAX) * 40.0f - 20.0f; 
        float y = (rand() / (float)RAND_MAX) * 3.0f + 0.5f;   
        float z = (rand() / (float)RAND_MAX) * 40.0f - 20.0f; 

        fireflies_.push_back(std::make_unique<Firefly>(
            this, sphereModel, constantShader, glm::vec3(x, y, z)
        ));
    }

}

void ForestScene::update(float time, Camera& camera) {

    for (auto& firefly : fireflies_) {
        firefly->update(time);
    }

    if (flashlightLight_) {
        // Устанавливаем позицию камеры
        flashlightLight_->setPosition(camera.getPosition());
        // Устанавливаем направление взгляда камеры
        flashlightLight_->setDirection(camera.getFront());
        // Флаг enabled_ управляется в Controller
    }

    updateSceneLights();

}