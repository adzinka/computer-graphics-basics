#pragma once
#include "Scene.h"

class TestScene : public Scene {
public:
    void setup(Camera& camera, ResourceManager& manager) override;
    void update(float time) override;
};