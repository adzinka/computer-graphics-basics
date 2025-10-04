#pragma once
#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>

class Scale : public TransformComponent {
public:
    Scale(const glm::vec3& s) : scale(s) {}
    void setScale(const glm::vec3& s) { scale = s; }

    glm::mat4 getMatrix() const override {
        return glm::scale(glm::mat4(1.0f), scale);
    }
private:
    glm::vec3 scale;
};