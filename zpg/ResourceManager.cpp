#include "ResourceManager.h"
#include <vector>       
#include <iostream>     

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

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

    model->enableAttrib(2, 2, GL_FLOAT, GL_FALSE, stride, 6 * sizeof(float));

    models_[name] = std::move(model);
}

void ResourceManager::loadModel(const std::string& name, const std::string& objFilePath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFilePath.c_str())) {
        std::cerr << "ERROR: Failed to load model " << name << ": " << warn + err << std::endl;
        return;
    }

    if (!warn.empty()) {
        std::cout << "WARNING: " << name << ": " << warn << std::endl;
    }

    std::vector<float> packedData;
    const size_t stride = 6 * sizeof(float); 

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            packedData.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            packedData.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            packedData.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            if (index.normal_index >= 0) {
                packedData.push_back(attrib.normals[3 * index.normal_index + 0]);
                packedData.push_back(attrib.normals[3 * index.normal_index + 1]);
                packedData.push_back(attrib.normals[3 * index.normal_index + 2]);
            }
            else {
                packedData.push_back(0.0f);
                packedData.push_back(1.0f); 
                packedData.push_back(0.0f);
            }
        }
    }

    auto model = std::make_unique<Model>();
    model->upload(packedData.data(), packedData.size() * sizeof(float), stride);


    model->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    model->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, stride, 3 * sizeof(float));

    models_[name] = std::move(model);
    std::cout << "Model " << name << " loaded successfully: "
        << (packedData.size() / 6) << " vertices.\n";
}

ShaderProgram* ResourceManager::getShader(const std::string& name) {
    return shaders_.at(name).get();
}

Model* ResourceManager::getModel(const std::string& name) {
    return models_.at(name).get();
}