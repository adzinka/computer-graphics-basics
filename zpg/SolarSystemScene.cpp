#include "SolarSystemScene.h"
#include "ResourceManager.h"

#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "CompositeTransform.h"

void SolarSystemScene::setup(Camera& camera, ResourceManager& manager) {

    addLight(std::make_unique<Light>(
        glm::vec3(1.0f, 1.0f, 1.0f),                
        glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),         
        glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),         
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),        
        1.0f,  
        0.0f, 
        0.0f  
    ));

    Model* sphereModel = manager.getModel("sphere");
    ShaderProgram* progConstant = manager.getShader("constant");
    ShaderProgram* progLighting = manager.getShader("lambert"); 

    DrawableObject* sun = addDrawable(sphereModel, progConstant, GL_TRIANGLES, sphereModel->getVertexCount());
    sun->setColor(glm::vec3(1.0f, 1.0f, 0.5f));

    auto sunTransform = std::make_unique<CompositeTransform>();
    sunTransform->add(std::make_unique<Scale>(glm::vec3(1.5f))); 
    sun->setTransform(std::move(sunTransform));

    DrawableObject* earth = addDrawable(sphereModel, progLighting, GL_TRIANGLES, sphereModel->getVertexCount());
    earth->setColor(glm::vec3(0.3f, 0.4f, 1.0f));

    auto earthTransform = std::make_unique<CompositeTransform>();
    earthTransform->add(std::make_unique<Scale>(glm::vec3(0.5f)));

    auto earthAxisRot = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    earthAxisRotation_ = earthAxisRot.get(); // Сохраняем указатель
    earthTransform->add(std::move(earthAxisRot)); // Передаём владение

    earthTransform->add(std::make_unique<Translate>(glm::vec3(5.0f, 0.0f, 0.0f)));

    auto earthOrbitRot = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    earthOrbitRotation_ = earthOrbitRot.get();
    earthTransform->add(std::move(earthOrbitRot));

    earth->setTransform(std::move(earthTransform));

    DrawableObject* moon = addDrawable(sphereModel, progLighting, GL_TRIANGLES, sphereModel->getVertexCount());
    moon->setColor(glm::vec3(0.5f, 0.5f, 0.5f));

    auto moonTransform = std::make_unique<CompositeTransform>();
    moonTransform->add(std::make_unique<Scale>(glm::vec3(0.2f)));
    moonTransform->add(std::make_unique<Translate>(glm::vec3(1.0f, 0.0f, 0.0f)));

    auto moonOrbitRot = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    moonOrbitRotation_ = moonOrbitRot.get();
    moonTransform->add(std::move(moonOrbitRot));

    auto moonEarthAxisRot = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    moonCopy_EarthAxis_ = moonEarthAxisRot.get();
    moonTransform->add(std::move(moonEarthAxisRot));

    moonTransform->add(std::make_unique<Translate>(glm::vec3(5.0f, 0.0f, 0.0f)));

    auto moonEarthOrbitRot = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    moonCopy_EarthOrbit_ = moonEarthOrbitRot.get();
    moonTransform->add(std::move(moonEarthOrbitRot));

    moon->setTransform(std::move(moonTransform));
}

void SolarSystemScene::update(float time, Camera& camera) {
    updateSceneLights();

    float earthOrbitSpeed = time * 20.0f;
    float earthAxisSpeed = time * 150.0f;
    float moonOrbitSpeed = time * 80.0f;

 
    if (earthOrbitRotation_) earthOrbitRotation_->setAngle(earthOrbitSpeed);
    if (earthAxisRotation_) earthAxisRotation_->setAngle(earthAxisSpeed);

    if (moonOrbitRotation_) moonOrbitRotation_->setAngle(moonOrbitSpeed);

    if (moonCopy_EarthOrbit_) moonCopy_EarthOrbit_->setAngle(earthOrbitSpeed);
    if (moonCopy_EarthAxis_) moonCopy_EarthAxis_->setAngle(earthAxisSpeed);
}