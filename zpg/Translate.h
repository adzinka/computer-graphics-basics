#pragma once
#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>

class Translate : public TransformComponent {
public:
    Translate(const glm::vec3& p) : position(p) {}
    void setPosition(const glm::vec3& p) { position = p; }

    glm::mat4 getMatrix() const override {
        return glm::translate(glm::mat4(1.0f), position);
    }
private:
    glm::vec3 position;
};