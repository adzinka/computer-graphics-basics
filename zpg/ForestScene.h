#pragma once
#include "Scene.h"
#include "Light.h"

class Firefly;
class Camera;

class ForestScene : public Scene {
public:
    ForestScene(); 
    ~ForestScene(); 

    void setup(Camera& camera, ResourceManager& manager) override;
    void update(float time, Camera& camera) override;

    Light* getFlashlightLight() const { return flashlightLight_; }

private:
    std::vector<std::unique_ptr<Firefly>> fireflies_;

    Light* flashlightLight_ = nullptr;
};