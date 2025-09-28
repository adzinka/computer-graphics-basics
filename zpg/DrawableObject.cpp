#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"

void DrawableObject::draw() const {
    program_->use();
    model_->draw(mode_, first_, count_);
}
