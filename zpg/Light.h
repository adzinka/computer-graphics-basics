#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

enum class LightType {
    Point = 0,      // Точечный (как светлячки)
    Directional = 1, // Направленный (как Солнце/Луна)
    Spot = 2         // Прожектор/Фонарик
};

class Light {
public:

    /*Light(const glm::vec3& position,
        const glm::vec4& ambient,
        const glm::vec4& diffuse,
        const glm::vec4& specular,
        float constant, float linear, float quadratic)
        : position_(position), ambient_(ambient), diffuse_(diffuse), specular_(specular),
        constant_(constant), linear_(linear), quadratic_(quadratic) {
    }*/

    Light(const glm::vec3& position,
        const glm::vec4& ambient,
        const glm::vec4& diffuse,
        const glm::vec4& specular,
        float constant, float linear, float quadratic,
        LightType type = LightType::Point, // Новый параметр
        const glm::vec3& direction = glm::vec3(0.0f), // Новый параметр
        float cutOff = 0.0f, float outerCutOff = 0.0f, // Новые параметры
        bool enabled = true) // Новый параметр
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

    void setDirection(const glm::vec3& direction); // Добавляем в .cpp
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

    glm::vec3 direction_; // Направление света
    float cutOff_;        // Косинус внутреннего угла Spot
    float outerCutOff_;   // Косинус внешнего угла Spot
    LightType lightType_; // Тип света
    bool enabled_;        // Состояние света

};