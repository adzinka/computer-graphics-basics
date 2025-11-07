#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <memory>

enum class LightType {
    Point = 0,      
    Directional = 1, 
    Spot = 2         
};

class Light {
public:

    struct AttenuationPreset {
        float constant;
        float linear;
        float quadratic;
    };

    static constexpr AttenuationPreset ATTENUATION_SHORT = { 1.0f, 0.7f, 1.8f };     
    static constexpr AttenuationPreset ATTENUATION_MEDIUM = { 1.0f, 0.22f, 0.20f }; 

    static constexpr float SPOTLIGHT_NARROW_INNER = 12.5f;   
    static constexpr float SPOTLIGHT_NARROW_OUTER = 17.5f;

    static std::unique_ptr<Light> createPointLight(
        const glm::vec3& position,
        const glm::vec4& diffuse,
        AttenuationPreset preset = ATTENUATION_MEDIUM,
        const glm::vec4& ambient = glm::vec4(0.1f),
        const glm::vec4& specular = glm::vec4(1.0f))
    {
        return std::make_unique<Light>(
            position, 
            ambient, 
            diffuse, 
            specular,
            preset.constant, preset.linear, preset.quadratic
        );
    }

    static std::unique_ptr<Light> createSpotlight(
        const glm::vec3& position,
        const glm::vec3& direction,
        const glm::vec4& diffuse,
        float innerAngleDegrees = SPOTLIGHT_NARROW_INNER,
        float outerAngleDegrees = SPOTLIGHT_NARROW_OUTER,
        AttenuationPreset preset = ATTENUATION_MEDIUM,
        bool enabled = true)
    {
        return std::make_unique<Light>(
            position,
            glm::vec4(0.0f), 
            diffuse,
            glm::vec4(1.0f),
            preset.constant, preset.linear, preset.quadratic,
            LightType::Spot,
            direction,
            glm::cos(glm::radians(innerAngleDegrees)),
            glm::cos(glm::radians(outerAngleDegrees)),
            enabled
        );
    }

    static std::unique_ptr<Light> createDirectionalLight(
        const glm::vec3& direction,
        const glm::vec4& ambient,
        const glm::vec4& diffuse)
    {
        return std::make_unique<Light>(
            glm::vec3(0.0f),
            ambient,
            diffuse,
            glm::vec4(0.0f),
            1.0f, 0.0f, 0.0f,
            LightType::Directional,
            direction
        );
    }


    Light(
        const glm::vec3& position,
        const glm::vec4& ambient,
        const glm::vec4& diffuse,
        const glm::vec4& specular,
        float constant, float linear, float quadratic,
        LightType type = LightType::Point,
        const glm::vec3& direction = glm::vec3(0.0f),
        float cutOff = 0.0f, float outerCutOff = 0.0f, 
        bool enabled = true
    ) 
        : position_(position), ambient_(ambient), diffuse_(diffuse), specular_(specular),
        constant_(constant), linear_(linear), quadratic_(quadratic),
        direction_(direction), cutOff_(cutOff), outerCutOff_(outerCutOff),
        lightType_(type), enabled_(enabled) {
    }

    void setPosition(const glm::vec3& position);

    const glm::vec3& getPosition() const { return position_; }
    const glm::vec4& getAmbient() const { return ambient_; }
    const glm::vec4& getDiffuse() const { return diffuse_; }
    const glm::vec4& getSpecular() const { return specular_; }
    float getConstant() const { return constant_; }  
    float getLinear() const { return linear_; }    
    float getQuadratic() const { return quadratic_; }

    void setDirection(const glm::vec3& direction); 
    const glm::vec3& getDirection() const { return direction_; }
    LightType getType() const { return lightType_; }
    void setType(LightType type) { lightType_ = type; }

    void setEnabled(bool enabled) { enabled_ = enabled; }
    bool isEnabled() const { return enabled_; }

    float getCutOff() const { return cutOff_; }
    float getOuterCutOff() const { return outerCutOff_; }

private:
    glm::vec3 position_;
    glm::vec4 ambient_;
    glm::vec4 diffuse_;
    glm::vec4 specular_;
    float constant_;   
    float linear_;     
    float quadratic_;  

    glm::vec3 direction_; 
    float cutOff_;        
    float outerCutOff_;   
    LightType lightType_; 
    bool enabled_;        

};