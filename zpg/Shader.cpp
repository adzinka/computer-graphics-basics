#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>

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

Shader::Shader(const std::string& filename, GLenum type) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Unable to open shader file " << filename << std::endl;
        shaderID_ = 0;
        return;
    }

    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    if (source.size() >= 3 &&
        source[0] == (char)0xEF &&
        source[1] == (char)0xBB &&
        source[2] == (char)0xBF)
    {
        source.erase(0, 3);
    }

    const char* sourceCStr = source.c_str();

    shaderID_ = glCreateShader(type);
    glShaderSource(shaderID_, 1, &sourceCStr, nullptr);
    glCompileShader(shaderID_);

    GLint ok = GL_FALSE;
    glGetShaderiv(shaderID_, GL_COMPILE_STATUS, &ok);
    if (ok == GL_FALSE) {
        GLint len = 0;
        glGetShaderiv(shaderID_, GL_INFO_LOG_LENGTH, &len);
        GLchar* log = new GLchar[len + 1];
        glGetShaderInfoLog(shaderID_, len, NULL, log);
        fprintf(stderr, "[Compile error] in %s:\n%s\n", filename.c_str(), log);
        delete[] log;

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
