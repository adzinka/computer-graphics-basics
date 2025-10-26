#pragma once
#include "Scene.h"

class TriangleScene : public Scene {
public:
	void setup(Camera& camera, ResourceManager& manager) override;
	void update(float time) override;

private:
	Rotate* triangleRotation_ = nullptr;
};