#include "Texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture() {}

Texture::~Texture() {
    if (textureID_) {
        glDeleteTextures(1, &textureID_);
        textureID_ = 0;
    }
}

bool Texture::load(const std::string& filepath) {
    if (textureID_) {
        glDeleteTextures(1, &textureID_);
        textureID_ = 0;
    }

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filepath.c_str(), &width_, &height_, &channels_, 4);

    if (!data) {
        std::cerr << "ERROR: Failed to load texture: " << filepath << std::endl;
        return false;
    }

    std::cout << "Texture loaded: " << filepath
        << " (" << width_ << "x" << height_
        << ", " << channels_ << " channels)" << std::endl;

    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::bind(GLuint textureUnit) const {
    if (textureID_) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureID_);
    }
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}