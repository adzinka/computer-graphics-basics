#include "Scene.h"
#include <iostream>

Model* Scene::makeModel(const void* vertices, GLsizeiptr sizeBytes) {
    auto model = std::make_unique<Model>();
    model->upload(vertices, sizeBytes);
    models_.push_back(std::move(model));
    return models_.back().get(); 
}

ShaderProgram* Scene::makeProgram(const char* vertexSrc, const char* fragmentSrc) {
    auto program = std::make_unique<ShaderProgram>(vertexSrc, fragmentSrc);

    if (!program->valid()) {
        std::cerr << "Shader program creation failed!" << std::endl;
        return nullptr; 
    }

    programs_.push_back(std::move(program));
    return programs_.back().get();
}

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