#pragma once

#include <vector>
#include <memory> 
#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Rotate.h"
#include "Light.h"
#include "Skybox.h"

class Camera;
class ResourceManager;

class Scene {
public:
    const static int MAX_LIGHTS = 8;
    virtual ~Scene() = default;

    virtual void setup(Camera& camera, ResourceManager& manager) = 0;
    virtual void update(float time, Camera& camera) = 0;

    virtual void drawAll(const Camera& camera) const;

    Light* addLight(std::unique_ptr<Light> light);
    DrawableObject* addDrawable(Model* model, ShaderProgram* program, GLenum mode, GLsizei count);
    virtual Light* getFlashlight() { return nullptr; }

protected:
    std::vector<std::unique_ptr<DrawableObject>> drawables_;

    std::vector<std::unique_ptr<Light>> lights_;

    std::unique_ptr<Skybox> skybox_ = nullptr;
    
    void updateSceneLights() const;

};
