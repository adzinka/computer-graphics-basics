#pragma once
#include "Scene.h"      
#include "Light.h"       
#include "DrawableObject.h" 
#include <glm/glm.hpp>

class Model;
class ShaderProgram;

class Firefly {
public:
    Firefly(Scene* scene, Model* model, ShaderProgram* shader, glm::vec3 basePosition);

    void update(float time);

private:
    Light* myLight_;          
    DrawableObject* myObject_;  

    glm::vec3 basePosition_; 
    float timeOffset_;       
};