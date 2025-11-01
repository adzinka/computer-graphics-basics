#include "Light.h"

void Light::setPosition(const glm::vec3& position) {
    position_ = position;
}

void Light::setDirection(const glm::vec3& direction) {
    direction_ = direction;
}
