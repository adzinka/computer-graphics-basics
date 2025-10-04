#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

void DrawableObject::draw() const {

    program_->use();

    glm::mat4 modelMatrix = transform_.getMatrix();

    program_->setUniform("modelMatrix", modelMatrix);

    model_->draw(mode_, first_, count_);
}
