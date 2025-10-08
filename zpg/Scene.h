#pragma once

#include <vector>
#include <memory> 
#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Rotate.h"

class Scene {
public:
    virtual ~Scene() = default;

    virtual void setup() = 0;
    virtual void update(float time) = 0;

    void drawAll() const;
  
protected:
    std::vector<std::unique_ptr<Model>> models_;
    std::vector<std::unique_ptr<ShaderProgram>> programs_;
    std::vector<std::unique_ptr<DrawableObject>> drawables_;

    Model* makeModel(const void* vertices, GLsizeiptr sizeBytes, GLsizei strideBytes);
    ShaderProgram* makeProgram(const char* vertexSrc, const char* fragmentSrc);
    DrawableObject* addDrawable(Model* model, ShaderProgram* program, GLenum mode, GLsizei count);
};
