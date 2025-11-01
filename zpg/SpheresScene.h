#pragma once

#include "Scene.h"

class SpheresScene : public Scene {
public: 
	void setup(Camera& camera, ResourceManager& manager) override;
	void update(float time, Camera& camera) override;

private:

};
