#pragma once
#include <map>
#include <string>
#include <memory>
#include "ShaderProgram.h"
#include "Model.h"
#include "Camera.h"

class ResourceManager {
public:
    void createModel(const std::string& name, const void* data, size_t dataSize, size_t stride, bool hasColor);

    void loadModel(const std::string& name, const std::string& objFilePath);

    void createShader(const std::string& name, const std::string& vsFile, const std::string& fsFile, Camera& camera);

    ShaderProgram* getShader(const std::string& name);
    Model* getModel(const std::string& name);

private:
    std::map<std::string, std::unique_ptr<ShaderProgram>> shaders_;
    std::map<std::string, std::unique_ptr<Model>> models_;

};
