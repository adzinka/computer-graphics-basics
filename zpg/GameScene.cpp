#include "GameScene.h"
#include "ResourceManager.h"
#include "LinearMovement.h"
#include "Scale.h"
#include "CompositeTransform.h"
#include <cstdlib> // rand()
#include <ctime>   // time()
#include <iostream>

GameScene::GameScene() {
    srand(static_cast<unsigned int>(time(NULL))); 
}

GameScene::~GameScene() {}

void GameScene::setup(Camera& camera, ResourceManager& manager) {
    
    targetModel_ = manager.getModel("duck"); 
    targetShader_ = manager.getShader("texture_light");
    targetTexture_ = manager.getTexture("duck_texture");

    Model* plain = manager.getModel("plain_uv");
    ShaderProgram* texShader = manager.getShader("texture_light");
    Texture* grass = manager.getTexture("grass");

    if (plain && texShader && grass) {
        DrawableObject* ground = addDrawable(plain, texShader, GL_TRIANGLES, plain->getVertexCount());
        ground->setTexture(grass);
        ground->setUvScale(10.0f);
        ground->setDeletable(false); 

        auto tr = std::make_unique<CompositeTransform>();
        tr->add(std::make_unique<Scale>(glm::vec3(20.0f)));
        ground->setTransform(std::move(tr));
    }

    addLight(Light::createDirectionalLight(
        glm::vec3(-0.5f, -1.0f, -0.5f),
        glm::vec4(0.4f), // Ambient
        glm::vec4(0.8f)  // Diffuse
    ));

    skybox_ = std::make_unique<Skybox>(
        manager.getModel("skybox_cube"),
        manager.getShader("skybox"),
        manager.getCubemap("mainSkybox")
    );

    printf("=== GAME START ===\nShoot the flying gifts!\n");
}

void GameScene::update(float time, Camera& camera) {
    static float lastTime = 0.0f;
    // if first frame
    if (lastTime == 0.0f) lastTime = time;

    float dt = time - lastTime;
    lastTime = time;

    if (dt > 0.1f) dt = 0.1f;

    timeSinceLastSpawn_ += dt;
    if (timeSinceLastSpawn_ > spawnInterval_) {
        spawnTarget();
        timeSinceLastSpawn_ = 0.0f;
        if (spawnInterval_ > 0.5f) spawnInterval_ *= 0.98f;
    }

    auto it = targets_.begin();
    while (it != targets_.end()) {
        if (!it->object || !it->movement) {
            it = targets_.erase(it);
            continue;
        }

        bool finished = it->movement->update(dt);

        if (finished) {
            int idToDelete = it->id;

            Scene::deleteObjectByID(idToDelete);

            it = targets_.erase(it);
        }
        else {
            ++it;
        }
    }

    updateSceneLights();
}

void GameScene::spawnTarget() {
    if (!targetModel_ || !targetShader_) return;

    // Random position Start
    float startX = (rand() % 20) - 10.0f; // -10 to 10
    float startY = 0.5f;
    float startZ = -15.0f; 

    // Random position End
    float endX = (rand() % 20) - 10.0f;
    float endY = (rand() % 5) + 1.0f; 
    float endZ = 5.0f; 

    glm::vec3 start(startX, startY, startZ);
    glm::vec3 end(endX, endY, endZ);
    float speed = 0.3f + (static_cast<float>(rand()) / RAND_MAX) * 0.5f; 

    DrawableObject* obj = addDrawable(targetModel_, targetShader_, GL_TRIANGLES, targetModel_->getVertexCount());
    obj->setDeletable(true);
    obj->setTexture(targetTexture_);

    auto moveComp = std::make_unique<LinearMovement>(start, end, speed);
    LinearMovement* movePtr = moveComp.get(); 

    auto composite = std::make_unique<CompositeTransform>();
    composite->add(std::make_unique<Scale>(glm::vec3(0.02f)));
    composite->add(std::make_unique<Rotate>(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
    composite->add(std::move(moveComp));

    obj->setTransform(std::move(composite));

    GameTarget target;
    target.object = obj;
    target.movement = movePtr;
    target.id = obj->getID();
    targets_.push_back(target);
}

void GameScene::deleteObjectByID(int id) {
    for (size_t i = 0; i < targets_.size(); ++i) {
        if (targets_[i].id == id) {
            score_++;
            printf("HIT!!! Score: %d\n", score_);

            targets_.erase(targets_.begin() + i);
            break; 
        }
    }

    Scene::deleteObjectByID(id);
}