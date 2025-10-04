#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

void DrawableObject::draw() const {
    /*program_->use();
    model_->draw(mode_, first_, count_);*/

    program_->use();
    float angle = glfwGetTime(); 

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    //modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); 
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f)); 
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f)); 

    program_->setUniformMat4("modelMatrix", modelMatrix);

    model_->draw(mode_, first_, count_);
}
