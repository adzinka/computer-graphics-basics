#pragma once

#include "Scene.h"

class SpheresScene : public Scene {
public: 
	void setup(Camera& camera, ResourceManager& manager) override;
	void update(float time) override;

private:
	std::vector<Rotate*> sphereRotations_;

};
