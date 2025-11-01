#pragma once
#include "Scene.h"

class TriangleScene : public Scene {
public:
	void setup(Camera& camera, ResourceManager& manager) override;
	void update(float time, Camera& camera) override;

private:
	Rotate* triangleRotation_ = nullptr;
};