#pragma once

#include "Scene.h"

class SpheresScene : public Scene {
public: 
	void setup() override;
	void update(float time) override;

private:
	std::vector<Rotate*> sphereRotations_;

};
