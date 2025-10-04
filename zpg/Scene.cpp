#include "Scene.h"
#include <iostream>

Model* Scene::makeModel(const void* vertices, GLsizeiptr sizeBytes, GLsizei strideBytes) {
    auto model = std::make_unique<Model>();

    model->upload(vertices, sizeBytes, strideBytes);

    models_.push_back(std::move(model));
    return models_.back().get(); 
}

ShaderProgram* Scene::makeProgram(const char* vertexSrc, const char* fragmentSrc) {
    Shader vertexShader(vertexSrc, GL_VERTEX_SHADER);
    Shader fragmentShader(fragmentSrc, GL_FRAGMENT_SHADER);

    auto program = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

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

void Scene::update(float time) {

    if (!drawables_.empty()) {
        drawables_[0]->getTransform().setRotation(glm::vec3(0.0f, time * 50.0f, 0.0f));
    }
    if (drawables_.size() > 1) {

        float red = (sin(time * 2.0f) / 2.0f) + 0.5f; 
        float green = (cos(time * 2.0f) / 2.0f) + 0.5f; 

        ShaderProgram* prog = drawables_[1]->getProgram();
        prog->useProgram(); 
        prog->setUniform("shapeColor", glm::vec4(red, green, 0.7f, 1.0f));

        drawables_[1]->getTransform().setRotation(glm::vec3(0.0f, -time * 50.0f, 0.0f));
    }
}