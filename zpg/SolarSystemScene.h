#pragma once
#include "Scene.h"

class Rotate;

class SolarSystemScene : public Scene {
public:
    void setup(Camera& camera, ResourceManager& manager) override;
    void update(float time, Camera& camera) override;

private:
    Rotate* earthOrbitRotation_ = nullptr;
    Rotate* earthAxisRotation_ = nullptr;
    Rotate* moonOrbitRotation_ = nullptr;
    Rotate* moonCopy_EarthOrbit_ = nullptr;
    Rotate* moonCopy_EarthAxis_ = nullptr;
};