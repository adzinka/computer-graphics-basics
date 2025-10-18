#pragma once
#include "Scene.h"

class ForestScene : public Scene {
public:
    void setup(Camera& camera, ResourceManager& manager) override;
    void update(float time) override;

private:
    Rotate* suziRotation_ = nullptr;
};