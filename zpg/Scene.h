#pragma once

#include <vector>
#include <memory> 
#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Rotate.h"

class Camera;
class ResourceManager;

class Scene {
public:
    virtual ~Scene() = default;

    virtual void setup(Camera& camera, ResourceManager& manager) = 0;
    virtual void update(float time) = 0;

    void drawAll() const;
  
protected:
    std::vector<std::unique_ptr<DrawableObject>> drawables_;

    DrawableObject* addDrawable(Model* model, ShaderProgram* program, GLenum mode, GLsizei count);
};
