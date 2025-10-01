#pragma once

#include <vector>
#include <memory> 
#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    Model* makeModel(const void* vertices, GLsizeiptr sizeBytes);
    ShaderProgram* makeProgram(const char* vertexSrc, const char* fragmentSrc);
    DrawableObject* addDrawable(Model* model, ShaderProgram* program, GLenum mode, GLsizei count);

    void drawAll() const;

private:
    std::vector<std::unique_ptr<Model>> models_;
    std::vector<std::unique_ptr<ShaderProgram>> programs_;
    std::vector<std::unique_ptr<DrawableObject>> drawables_;
};
