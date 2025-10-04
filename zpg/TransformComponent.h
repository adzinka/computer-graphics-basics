#pragma once
#include <glm/glm.hpp>

class TransformComponent {
public:
    virtual ~TransformComponent() = default;
    virtual glm::mat4 getMatrix() const = 0;
};
