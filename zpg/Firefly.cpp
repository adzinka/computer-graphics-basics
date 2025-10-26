#include "Firefly.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Translate.h"
#include "Scale.h"
#include "CompositeTransform.h"

Firefly::Firefly(Scene* scene, Model* model, ShaderProgram* shader, glm::vec3 basePosition)
    : basePosition_(basePosition)
{

    auto light = std::make_unique<Light>(
        basePosition,                       
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  
        glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),  
        glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),  
        0.5f,                               
        2.5f,                               
        4.0f                                
    );
    myLight_ = scene->addLight(std::move(light));

    myObject_ = scene->addDrawable(model, shader, GL_TRIANGLES, model->getVertexCount());
    myObject_->setColor(glm::vec3(1.0f, 1.0f, 0.5f)); 

    timeOffset_ = (rand() / (float)RAND_MAX) * 1000.0f;

    update(0.0f); 
}

void Firefly::update(float time) {
    float t = time + timeOffset_;

    glm::vec3 offset = glm::vec3(
        sin(t * 1.3f) * 0.7f, 
        cos(t * 0.8f) * 0.4f, 
        cos(t * 1.1f) * 0.6f 
    );

    glm::vec3 newPos = basePosition_ + offset;

    myLight_->setPosition(newPos);

    auto transform = std::make_unique<CompositeTransform>();
    transform->add(std::make_unique<Scale>(glm::vec3(0.05f)));
    transform->add(std::make_unique<Translate>(newPos));
    myObject_->setTransform(std::move(transform));
}