#pragma once

// GLEW
#include <GL/glew.h>

// GLM 
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <string>
#include "Shader.h"

class Camera;

class ShaderProgram {
public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();

    void useProgram() const;
    void unuseProgram() const;

    bool valid() const { return program_ != 0; }

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void setUniform(const char* uniformName, const glm::mat4& matrix) const;
    void setUniform(const char* uniformName, float value) const;
    void setUniform(const char* uniformName, int value) const;
    void setUniform(const char* uniformName, const glm::vec2& vector) const;
    void setUniform(const char* uniformName, const glm::vec3& vector) const;
    void setUniform(const char* uniformName, const glm::vec4& vector) const;
    void update(const Camera& camera);

private:
    GLuint program_ = 0;

    static bool link(GLuint program, const char* tag);
};
