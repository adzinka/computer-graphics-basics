#pragma once

// GLEW
#include <GL/glew.h>

// GLM 
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <string>

class ShaderProgram {
public:
    ShaderProgram(const char* vertexSrc, const char* fragmentSrc);
    ~ShaderProgram();

    void use() const;
    void unuse() const;

    bool valid() const { return program_ != 0; }

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

private:
    GLuint program_ = 0;

    static bool compile(GLuint shader, const char* tag);
    static bool link(GLuint program, const char* tag);

};
