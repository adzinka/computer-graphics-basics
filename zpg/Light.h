#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

class Light {
public:

    Light(const glm::vec3& position,
        const glm::vec4& ambient,
        const glm::vec4& diffuse,
        const glm::vec4& specular,
        float constant, float linear, float quadratic)
        : position_(position), ambient_(ambient), diffuse_(diffuse), specular_(specular),
        constant_(constant), linear_(linear), quadratic_(quadratic) {
    }

    void setPosition(const glm::vec3& position);

    const glm::vec3& getPosition() const { return position_; }
    const glm::vec4& getAmbient() const { return ambient_; }
    const glm::vec4& getDiffuse() const { return diffuse_; }
    const glm::vec4& getSpecular() const { return specular_; }
    float getConstant() const { return constant_; }  
    float getLinear() const { return linear_; }    
    float getQuadratic() const { return quadratic_; }


private:
    glm::vec3 position_;
    glm::vec4 ambient_;
    glm::vec4 diffuse_;
    glm::vec4 specular_;
    float constant_;   
    float linear_;     
    float quadratic_;  

};