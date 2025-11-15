#pragma once

#include <GL/glew.h>
#include "Model.h"
#include "ShaderProgram.h"
#include "Camera.h"

class Skybox {
public:
    Skybox(Model* model, ShaderProgram* shader, GLuint cubemapTexID);

    void draw(const Camera& camera) const;

private:
    Model* model_;
    ShaderProgram* shader_;
    GLuint cubemapTexID_;
};