#pragma once
#include <GL/glew.h>
#include <string>

class Texture {
public:
    Texture();
    ~Texture();

    bool load(const std::string& filepath);

    void bind(GLuint textureUnit = 0) const;
    void unbind() const;

    GLuint getID() const { return textureID_; }
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    bool isLoaded() const { return textureID_ != 0; }

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

private:
    GLuint textureID_ = 0;
    int width_ = 0;
    int height_ = 0;
    int channels_ = 0;
};