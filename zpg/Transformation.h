#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformation {
public:
    Transformation()
        : position(0.0f),
        rotation(0.0f),
        scale(1.0f) {
    }

    void setPosition(const glm::vec3& pos) { position = pos; }
    void setRotation(const glm::vec3& rot) { rotation = rot; }
    void setScale(const glm::vec3& scl) { scale = scl; }

    glm::mat4 getMatrix() const;

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};