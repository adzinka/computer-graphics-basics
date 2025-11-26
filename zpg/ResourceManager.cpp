#include "ResourceManager.h"
#include <vector>       
#include <iostream>     

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "stb_image.h"

void ResourceManager::createShader(const std::string& name, const std::string& vsFile, const std::string& fsFile, Camera* camera) {
    Shader vertexShader(vsFile, GL_VERTEX_SHADER);
    Shader fragmentShader(fsFile, GL_FRAGMENT_SHADER);

    auto shaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
    
    if (camera) {
        camera->addObserver(shaderProgram.get());
        shaderProgram->update(*camera);
    }

    shaders_[name] = std::move(shaderProgram);
}

void ResourceManager::createModel(const std::string& name, const void* data, size_t dataSize, size_t stride, bool hasColor) {
    auto model = std::make_unique<Model>();
    model->upload(data, dataSize, stride);

    model->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

    if (hasColor) {
        model->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, stride, 3 * sizeof(float));
    }

    if (stride >= 8 * sizeof(float)) {
        model->enableAttrib(2, 2, GL_FLOAT, GL_FALSE, stride, 6 * sizeof(float));
    }

    models_[name] = std::move(model);
}

void ResourceManager::loadModel(const std::string& name, const std::string& objFilePath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::string mtlDir = "./";
    size_t lastSlash = objFilePath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        mtlDir = objFilePath.substr(0, lastSlash + 1);
    }

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
        objFilePath.c_str(), mtlDir.c_str())) {
        std::cerr << "ERROR: Failed to load model " << name << ": " << warn + err << std::endl;
        return;
    }

    if (!warn.empty()) {
        std::cout << "WARNING: " << name << ": " << warn << std::endl;
    }

    bool hasUV = !attrib.texcoords.empty();

    std::vector<float> packedData;
    size_t stride;
    size_t floatsPerVertex;

    if (hasUV) {
        // Stride: 3 (position) + 3 (normal) + 2 (uv) = 8 floats
        stride = 8 * sizeof(float);
        floatsPerVertex = 8;
    }
    else {
        // Stride: 3 (position) + 3 (normal) = 6 floats
        stride = 6 * sizeof(float);
        floatsPerVertex = 6;
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            // 1. Position (xyz)
            packedData.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            packedData.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            packedData.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            // 2. Normal (xyz)
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

            // 3. UV coordinates
            if (hasUV) {
                if (index.texcoord_index >= 0) {
                    packedData.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                    packedData.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
                }
                else {
                    packedData.push_back(0.0f);
                    packedData.push_back(0.0f);
                }
            }
        }
    }

    auto model = std::make_unique<Model>();
    model->upload(packedData.data(), packedData.size() * sizeof(float), stride);

    // location = 0: position (vec3)
    model->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

    // location = 1: normal (vec3)
    model->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, stride, 3 * sizeof(float));

    // location = 2: UV (vec2)
    if (hasUV) {
        model->enableAttrib(2, 2, GL_FLOAT, GL_FALSE, stride, 6 * sizeof(float));
    }

    models_[name] = std::move(model);

    if (hasUV) {
        std::cout << "Model " << name << " loaded: "
            << (packedData.size() / floatsPerVertex) << " vertices (with UV).\n";
    }
    else {
        std::cout << "Model " << name << " loaded: "
            << (packedData.size() / floatsPerVertex) << " vertices (NO UV).\n";
    }
}

void ResourceManager::loadTexture(const std::string& name, const std::string& filepath) {
    auto texture = std::make_unique<Texture>();

    if (texture->load(filepath)) {
        textures_[name] = std::move(texture);
        std::cout << "Texture '" << name << "' loaded successfully.\n";
    }
    else {
        std::cerr << "ERROR: Failed to load texture '" << name << "' from " << filepath << "\n";
    }
}

void ResourceManager::loadCubemap(const std::string& name, const std::vector<std::string>& faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            std::cout << "Cubemap face loaded: " << faces[i] << std::endl;
        }
        else
        {
            std::cerr << "ERROR: Failed to load cubemap texture face: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    cubemaps_[name] = textureID;
}

ShaderProgram* ResourceManager::getShader(const std::string& name) {
    return shaders_.at(name).get();
}

Model* ResourceManager::getModel(const std::string& name) {
    return models_.at(name).get();
}

Texture* ResourceManager::getTexture(const std::string& name) {
    auto it = textures_.find(name);
    if (it == textures_.end()) {
        std::cerr << "ERROR: Texture '" << name << "' not found!\n";
        return nullptr;
    }
    return it->second.get();
}

GLuint ResourceManager::getCubemap(const std::string& name) {
    auto it = cubemaps_.find(name);
    if (it == cubemaps_.end()) {
        std::cerr << "ERROR: Cubemap '" << name << "' not found!\n";
        return 0;
    }
    return it->second;
}