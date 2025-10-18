#include "ResourceManager.h"

void ResourceManager::createShader(const std::string& name, const char* vsSrc, const char* fsSrc, Camera& camera) {
    Shader vertexShader(vsSrc, GL_VERTEX_SHADER);
    Shader fragmentShader(fsSrc, GL_FRAGMENT_SHADER);

    auto shaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

    camera.addObserver(shaderProgram.get());
    shaderProgram->update(camera);

    shaders_[name] = std::move(shaderProgram);
}

void ResourceManager::createShader(const std::string& name, const std::string& vsFile, const std::string& fsFile, Camera& camera) {
    Shader vertexShader(vsFile, GL_VERTEX_SHADER);
    Shader fragmentShader(fsFile, GL_FRAGMENT_SHADER);

    auto shaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
    camera.addObserver(shaderProgram.get());
    shaderProgram->update(camera);
    shaders_[name] = std::move(shaderProgram);
}

void ResourceManager::createModel(const std::string& name, const void* data, size_t dataSize, size_t stride, bool hasColor) {
    auto model = std::make_unique<Model>();
    model->upload(data, dataSize, stride);

    model->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

    if (hasColor) {
        model->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, stride, 3 * sizeof(float));
    }

    models_[name] = std::move(model);
}

ShaderProgram* ResourceManager::getShader(const std::string& name) {
    return shaders_.at(name).get();
}

Model* ResourceManager::getModel(const std::string& name) {
    return models_.at(name).get();
}