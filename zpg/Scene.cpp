#include "Scene.h"
#include <iostream>

#include "Translate.h" 
#include "Scale.h"
#include "sphere.h"
#include "Camera.h"

DrawableObject* Scene::addDrawable(Model* model, ShaderProgram* program, GLenum mode, GLsizei count) {
    auto drawable = std::make_unique<DrawableObject>(model, program, mode, count);
    drawables_.push_back(std::move(drawable));
    return drawables_.back().get();
}

void Scene::drawAll() const {
    for (const auto& obj : drawables_) {
        obj->draw();
    }
}
