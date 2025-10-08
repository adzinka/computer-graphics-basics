#pragma once
#include "Scene.h"

class TriangleScene : public Scene {
public:
	void setup() override;
	void update(float time) override;

private:
	Rotate* triangleRotation_ = nullptr;
};