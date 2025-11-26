#pragma once
#include "TransformComponent.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

class LinearMovement : public TransformComponent {
public:
    LinearMovement(const glm::vec3& start, const glm::vec3& end, float speed)
        : start_(start), end_(end), speed_(speed) {
    }


    bool update(float dt) {
        progress_ += dt * speed_;
        if (progress_ > 1.0f) {
            progress_ = 1.0f;
            return true; 
        }
        return false;
    }

    glm::mat4 getMatrix() const override {
        // P(t) = A + t * (B - A)
        glm::vec3 currentPos = start_ + progress_ * (end_ - start_);
        return glm::translate(glm::mat4(1.0f), currentPos);
    }

private:
    glm::vec3 start_;
    glm::vec3 end_;
    float speed_;
    float progress_ = 0.0f; 
};