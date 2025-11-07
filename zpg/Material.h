#pragma once
#include <glm/glm.hpp>
#include <string>

namespace tinyobj {
    struct material_t;
}

class Material {
public:
    Material()
        : ambient_(0.2f, 0.2f, 0.2f),
        diffuse_(0.8f, 0.8f, 0.8f),
        specular_(0.5f, 0.5f, 0.5f),
        shininess_(32.0f)
    {
    }

    Material(const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float shininess)
        : ambient_(ambient),
        diffuse_(diffuse),
        specular_(specular),
        shininess_(shininess)
    {
    }

    const glm::vec3& getAmbient() const { return ambient_; }
    const glm::vec3& getDiffuse() const { return diffuse_; }
    const glm::vec3& getSpecular() const { return specular_; }
    float getShininess() const { return shininess_; }

    void setAmbient(const glm::vec3& ambient) { ambient_ = ambient; }
    void setDiffuse(const glm::vec3& diffuse) { diffuse_ = diffuse; }
    void setSpecular(const glm::vec3& specular) { specular_ = specular; }
    void setShininess(float shininess) { shininess_ = shininess; }

    void loadFromTinyOBJ(const tinyobj::material_t& mat);
    
    static Material createEmerald();
    static Material createGold();
    static Material createRuby();
    static Material createChrome();
    static Material createPlastic();

private:
    glm::vec3 ambient_;   
    glm::vec3 diffuse_;   
    glm::vec3 specular_;  
    float shininess_;    
};
