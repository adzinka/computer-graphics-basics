#pragma once
#include "Scene.h"
#include <vector>

class LinearMovement;

struct GameTarget {
    DrawableObject* object;      
    LinearMovement* movement;   
    int id;                      
};

class GameScene : public Scene {
public:
    GameScene();
    ~GameScene();

    void setup(Camera& camera, ResourceManager& manager) override;
    void update(float time, Camera& camera) override;

    bool allowsObjectPicking() const override { return true; }

    void deleteObjectByID(int id) override;
    
private:
    void spawnTarget();

    std::vector<GameTarget> targets_;

    float timeSinceLastSpawn_ = 0.0f;
    float spawnInterval_ = 1.0f; 

    Model* targetModel_ = nullptr;
    ShaderProgram* targetShader_ = nullptr;
    Texture* targetTexture_ = nullptr;

    int score_ = 0;
};