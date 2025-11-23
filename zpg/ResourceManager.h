#pragma once

#include <GL/glew.h>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "ShaderProgram.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"

class ResourceManager {
public:
    void createModel(const std::string& name, const void* data, size_t dataSize, size_t stride, bool hasColor);
    void loadModel(const std::string& name, const std::string& objFilePath);
    void createShader(const std::string& name, const std::string& vsFile, const std::string& fsFile, Camera* camera = nullptr);
    void loadTexture(const std::string& name, const std::string& filepath);

    ShaderProgram* getShader(const std::string& name);
    Model* getModel(const std::string& name);
    Texture* getTexture(const std::string& name);

    void loadCubemap(const std::string& name, const std::vector<std::string>& faces);
    GLuint getCubemap(const std::string& name);

private:
    std::map<std::string, std::unique_ptr<ShaderProgram>> shaders_;
    std::map<std::string, std::unique_ptr<Model>> models_;
    std::map<std::string, std::unique_ptr<Texture>> textures_;
    std::map<std::string, GLuint> cubemaps_;
};
