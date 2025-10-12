#pragma once
#include "Scene.h"

class ForestScene : public Scene {
public:
    void setup(Camera& camera) override;
    void update(float time) override;

private:
    Rotate* suziRotation_ = nullptr;
};