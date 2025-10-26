#pragma once
#include "Scene.h"

class Firefly;

class ForestScene : public Scene {
public:
    ForestScene(); 
    ~ForestScene(); 

    void setup(Camera& camera, ResourceManager& manager) override;
    void update(float time) override;

private:
    std::vector<std::unique_ptr<Firefly>> fireflies_;
};