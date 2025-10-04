#pragma once
#include <GL/glew.h>
#include <string>

class Shader {
public:
    Shader(const char* source, GLenum type);
    ~Shader();

    void attachTo(GLuint programID) const;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

private:
    GLuint shaderID_ = 0;
};