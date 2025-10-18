#pragma once
#include <GL/glew.h>
#include <string>

class Shader {
public:
    Shader(const char* source, GLenum type);
    Shader(const std::string& filename, GLenum type);

    ~Shader();

    void attachTo(GLuint programID) const;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    void createShader(GLenum shaderType, const char* shaderCode);
    void createShaderFromFile(GLenum shaderType, const char* shaderFile);

private:
    GLuint shaderID_ = 0;

};