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

        if (texture_ && texture_->isLoaded()) {
            texture_->bind(0);  
            program_->setUniform("textureUnitID", 0);
            program_->setUniform("useTexture", 1);
        }
        else {
            program_->setUniform("useTexture", 0);
        }

        model_->draw(mode_, first_, count_);

        if (texture_) {
            texture_->unbind();
        }

        program_->unuseProgram();
    }
}

void DrawableObject::setTransform(std::unique_ptr<TransformComponent> transform) {
    transform_ = std::move(transform);
}
