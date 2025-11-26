#include "Scene.h"
#include "Texture.h"
#include "Translate.h" 
#include "Scale.h"
#include "Camera.h"
#include <set>
#include <string>
#include <iostream>

Scene::~Scene() = default;

DrawableObject* Scene::addDrawable(Model* model, ShaderProgram* program, GLenum mode, GLsizei count) {
    auto drawable = std::make_unique<DrawableObject>(model, program, mode, count);
    
    int newID = 1;
    while (true) {
        bool isUsed = false;
     
        for (const auto& obj : drawables_) {
            if (obj->getID() == newID) {
                isUsed = true;
                break;
            }
        }
        if (!isUsed) break; 
        newID++; 
    }

    if (newID <= 255) {
        drawable->setID(newID);
    }
    else {
        printf("WARNING: Run out of IDs (max 255)!\n");
    }

    drawables_.push_back(std::move(drawable));
    return drawables_.back().get();
}

void Scene::drawAll(const Camera& camera) const {
    for (const auto& obj : drawables_) {
        obj->draw();
    }

    if (skybox_) {
        skybox_->draw(camera);
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

            shader->setUniform((base + "direction").c_str(), light->getDirection());
            shader->setUniform((base + "cutOff").c_str(), light->getCutOff());
            shader->setUniform((base + "outerCutOff").c_str(), light->getOuterCutOff());
            shader->setUniform((base + "type").c_str(), static_cast<int>(light->getType()));
            shader->setUniform((base + "enabled").c_str(), light->isEnabled() ? 1 : 0);
        }

        shader->unuseProgram();
    }
}

DrawableObject* Scene::getObjectByID(int id) {
    if (id <= 0) return nullptr;

    for (const auto& obj : drawables_) {
        if (obj && obj->getID() == id) {
            return obj.get();
        }
    }
    return nullptr;
}

void Scene::deleteObjectByID(int id) {
    if (id <= 0) return;

    auto it = std::remove_if(drawables_.begin(), drawables_.end(), [id](const auto& obj) {
        return obj && obj->getID() == id;
        });

    if (it != drawables_.end()) {
        drawables_.erase(it, drawables_.end());
    }
}

void Scene::addObject(Model* model, ShaderProgram* program, const glm::vec3& position) {
    DrawableObject* obj = addDrawable(model, program, GL_TRIANGLES, model->getVertexCount());

    obj->setColor(glm::vec3(0.1f, 0.4f, 0.1f));

    auto objComposite = std::make_unique<CompositeTransform>();
    objComposite->add(std::make_unique<Scale>(glm::vec3(1.0f))); 
    objComposite->add(std::make_unique<Translate>(position));
    obj->setTransform(std::move(objComposite));
}

void Scene::addCharacter(Model* model, ShaderProgram* shader, Texture* texture, const glm::vec3& position) {
    DrawableObject* obj = addDrawable(model, shader, GL_TRIANGLES, model->getVertexCount());

    obj->setTexture(texture); 
    obj->setUvScale(1.0f);    
    obj->setDeletable(true); 

    auto objComposite = std::make_unique<CompositeTransform>();
    objComposite->add(std::make_unique<Scale>(glm::vec3(1.0f)));
    objComposite->add(std::make_unique<Translate>(position));
    obj->setTransform(std::move(objComposite));
}