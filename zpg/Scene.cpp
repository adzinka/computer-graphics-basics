#include "Scene.h"
#include <iostream>

#include "Translate.h" 
#include "Scale.h"
#include "sphere.h"
#include "Camera.h"
#include <set>
#include <string>

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

Light* Scene::addLight(std::unique_ptr<Light> light) {
    Light* ptr = light.get();
    lights_.push_back(std::move(light));
    return ptr;
}

void Scene::updateSceneLights() const {
   
    int numLights = static_cast<int>(lights_.size());
    
    int lightCountToSend = (numLights < MAX_LIGHTS) ? numLights : MAX_LIGHTS;

    std::set<ShaderProgram*> shaders;
    for (const auto& drawable : drawables_) {
        if (drawable) {
            shaders.insert(drawable->getProgram());
        }
    }

    for (ShaderProgram* shader : shaders)
    {
        if (!shader) continue;

        shader->useProgram();

        shader->setUniform("numberOfLights", lightCountToSend);

        for (int i = 0; i < lightCountToSend; ++i)
        {
            const Light* light = lights_[i].get();

            std::string base = "lights[" + std::to_string(i) + "].";

            shader->setUniform((base + "position").c_str(),
                glm::vec4(light->getPosition(), 1.0f));

            shader->setUniform((base + "ambient").c_str(), light->getAmbient());
            shader->setUniform((base + "diffuse").c_str(), light->getDiffuse());
            shader->setUniform((base + "specular").c_str(), light->getSpecular());
            shader->setUniform((base + "constant").c_str(), light->getConstant());
            shader->setUniform((base + "linear").c_str(), light->getLinear());
            shader->setUniform((base + "quadratic").c_str(), light->getQuadratic());
        }
    }
}
