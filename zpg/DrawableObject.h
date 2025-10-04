#pragma once
#include <GL/glew.h>
#include "CompositeTransform.h"

class Model;
class ShaderProgram;

class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* program,
        GLenum mode, GLsizei count, GLint first = 0)
        : model_(model), program_(program),
        mode_(mode), count_(count), first_(first) {
    }

    void draw() const;

    /*Transformation& getTransform() { return transform_; }*/
    ShaderProgram* getProgram() const { return program_; }
    CompositeTransform& getTransform() { return transform_; }

private:
    Model* model_;   
    ShaderProgram* program_;  
    GLenum   mode_;
    GLsizei  count_;
    GLint    first_;
    CompositeTransform transform_;
};
