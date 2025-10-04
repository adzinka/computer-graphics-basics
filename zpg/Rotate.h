#pragma once
#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>

class Rotate : public TransformComponent {
public:
    Rotate(float a, const glm::vec3& ax) : angle(a), axis(ax) {}
    void setAngle(float a) { angle = a; }

    glm::mat4 getMatrix() const override {
        return glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    }
private:
    float angle; // ?hel ve stupn?ch
    glm::vec3 axis;
};