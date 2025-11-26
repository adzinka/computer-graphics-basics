#include "SolarSystemScene.h"
#include "ResourceManager.h"

#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "CompositeTransform.h"

void SolarSystemScene::setup(Camera& camera, ResourceManager& manager) {

 
    auto sunLight = std::make_unique<Light>(
        glm::vec3(0.0f, 0.0f, 0.0f),       
        glm::vec4(0.05f),                  // Ambient
        glm::vec4(1.0f, 1.0f, 0.9f, 1.0f), // Diffuse
        glm::vec4(0.0f),                   // Specular
        1.0f, 0.0f, 0.0f                  
    );

    addLight(std::move(sunLight));
    Model* planetModel = manager.getModel("planet");

    // without light
    ShaderProgram* progSun = manager.getShader("texture");     
    // with light
    ShaderProgram* progPlanets = manager.getShader("texture_light"); 

    DrawableObject* sun = addDrawable(planetModel, progSun, GL_TRIANGLES, planetModel->getVertexCount());
    sun->setTexture(manager.getTexture("sun_texture")); 

    auto sunTransform = std::make_unique<CompositeTransform>();
    sunTransform->add(std::make_unique<Scale>(glm::vec3(1.5f)));
    sun->setTransform(std::move(sunTransform));

    DrawableObject* earth = addDrawable(planetModel, progPlanets, GL_TRIANGLES, planetModel->getVertexCount());
    earth->setTexture(manager.getTexture("earth_texture"));

    auto earthTransform = std::make_unique<CompositeTransform>();
    earthTransform->add(std::make_unique<Scale>(glm::vec3(0.5f)));

    auto earthAxisRot = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    earthAxisRotation_ = earthAxisRot.get();
    earthTransform->add(std::move(earthAxisRot));

    earthTransform->add(std::make_unique<Translate>(glm::vec3(5.0f, 0.0f, 0.0f)));

    auto earthOrbitRot = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    earthOrbitRotation_ = earthOrbitRot.get();
    earthTransform->add(std::move(earthOrbitRot));

    earth->setTransform(std::move(earthTransform));

    DrawableObject* moon = addDrawable(planetModel, progPlanets, GL_TRIANGLES, planetModel->getVertexCount());
    moon->setTexture(manager.getTexture("moon_texture"));

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

    DrawableObject* mars = addDrawable(planetModel, progPlanets, GL_TRIANGLES, planetModel->getVertexCount());
    mars->setTexture(manager.getTexture("mars_texture"));

    auto marsTransform = std::make_unique<CompositeTransform>();
    marsTransform->add(std::make_unique<Scale>(glm::vec3(0.4f)));

    auto marsAxisRot = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    marsAxisRotation_ = marsAxisRot.get();
    marsTransform->add(std::move(marsAxisRot));

    marsTransform->add(std::make_unique<Translate>(glm::vec3(8.0f, 0.0f, 0.0f)));

    auto marsOrbitRot = std::make_unique<Rotate>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    marsOrbitRotation_ = marsOrbitRot.get();
    marsTransform->add(std::move(marsOrbitRot));

    mars->setTransform(std::move(marsTransform));
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

    if (marsOrbitRotation_) marsOrbitRotation_->setAngle(time * 12.0f);
    if (marsAxisRotation_) marsAxisRotation_->setAngle(time * 150.0f);
}