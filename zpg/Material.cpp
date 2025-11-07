#include "Material.h"
#include "tiny_obj_loader.h"

void Material::loadFromTinyOBJ(const tinyobj::material_t& mat) {
    ambient_ = glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]);

    diffuse_ = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);

    specular_ = glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]);

    shininess_ = mat.shininess;
    if (shininess_ > 128.0f) {
        shininess_ = shininess_ / 1000.0f * 128.0f;
    }
}

Material Material::createEmerald() {
    return Material(
        glm::vec3(0.0215f, 0.1745f, 0.0215f),   
        glm::vec3(0.07568f, 0.61424f, 0.07568f),
        glm::vec3(0.633f, 0.727811f, 0.633f),   
        76.8f                                   
    );
}

Material Material::createGold() {
    return Material(
        glm::vec3(0.24725f, 0.1995f, 0.0745f),
        glm::vec3(0.75164f, 0.60648f, 0.22648f),
        glm::vec3(0.628281f, 0.555802f, 0.366065f),
        51.2f
    );
}

Material Material::createRuby() {
    return Material(
        glm::vec3(0.1745f, 0.01175f, 0.01175f),
        glm::vec3(0.61424f, 0.04136f, 0.04136f),
        glm::vec3(0.727811f, 0.626959f, 0.626959f),
        76.8f
    );
}

Material Material::createChrome() {
    return Material(
        glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.774597f, 0.774597f, 0.774597f),
        76.8f
    );
}

Material Material::createPlastic() {
    return Material(
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        32.0f
    );
}