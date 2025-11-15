#pragma once
#include "Scene.h"
#include "Light.h"
#include <memory>

class Firefly;
class Camera;
class Model;
class ShaderProgram;

class ForestScene : public Scene {
public:
    ForestScene();
    ~ForestScene();

    void setup(Camera& camera, ResourceManager& manager) override;
    void update(float time, Camera& camera) override;

    Light* getFlashlight() override { return flashlightLight_; }

private:

    std::vector<std::unique_ptr<Firefly>> fireflies_;

    Light* flashlightLight_ = nullptr;
    Camera* camera_ = nullptr;
};