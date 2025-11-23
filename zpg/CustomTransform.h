#pragma once
#include "TransformComponent.h"

class CustomTransform : public TransformComponent {
public:
    glm::mat4 getMatrix() const override {
        glm::mat4 matrix = glm::mat4(1.0f); 
        matrix[3][3] = 20.0f;               // Change w
        return matrix;
    }
};