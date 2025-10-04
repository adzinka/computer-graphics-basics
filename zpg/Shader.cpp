#include "Shader.h"
#include <iostream>

Shader::Shader(const char* source, GLenum type) {
    shaderID_ = glCreateShader(type);
    glShaderSource(shaderID_, 1, &source, nullptr);
    glCompileShader(shaderID_);

    GLint ok = GL_FALSE;
    glGetShaderiv(shaderID_, GL_COMPILE_STATUS, &ok);
    if (ok == GL_FALSE) {
        fprintf(stderr, "Shader compilation failed!\n");
        glDeleteShader(shaderID_);
        shaderID_ = 0;
    }
}

Shader::~Shader() {
    if (shaderID_) {
        glDeleteShader(shaderID_);
    }
}

void Shader::attachTo(GLuint programID) const {
    if (shaderID_ != 0) {
        glAttachShader(programID, shaderID_);
    }
}