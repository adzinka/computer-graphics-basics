#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

void DrawableObject::draw() const {

    if (model_ && program_) {
        program_->useProgram();

        glm::mat4 modelMatrix = transform_ ? transform_->getMatrix() : glm::mat4(1.0f);
        program_->setUniform("modelMatrix", modelMatrix);
        program_->setUniform("objectColor", color_); 

        model_->draw(mode_, first_, count_);

        program_->unuseProgram();
    }
}

void DrawableObject::setTransform(std::unique_ptr<TransformComponent> transform) {
    transform_ = std::move(transform);
}
