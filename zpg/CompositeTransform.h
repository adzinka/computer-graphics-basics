#pragma once
#include "TransformComponent.h"
#include <vector>
#include <memory>

class CompositeTransform : public TransformComponent {
public:
    void add(std::unique_ptr<TransformComponent> component) {
        components_.push_back(std::move(component));
    }

    glm::mat4 getMatrix() const override {
        glm::mat4 matrix = glm::mat4(1.0f);
        for (const auto& component : components_) {
            matrix = component->getMatrix() * matrix;
        }
        return matrix;
    }

private:
    std::vector<std::unique_ptr<TransformComponent>> components_;
};