#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader)
{
    program_ = glCreateProgram();
    
    vertexShader.attachTo(program_);
    fragmentShader.attachTo(program_);

    glLinkProgram(program_);


    if (!link(program_, "PROGRAM")) {
        glDeleteProgram(program_);
        program_ = 0;
    }
}

ShaderProgram::~ShaderProgram() {
    if (program_) glDeleteProgram(program_);
}

void ShaderProgram::useProgram() const {
    if (program_) glUseProgram(program_);
}

void ShaderProgram::unuseProgram() const {
    glUseProgram(0);
}

bool ShaderProgram::compile(GLuint shader, const char* tag) {
    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (ok == GL_FALSE) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        GLchar* log = new GLchar[len + 1];
        glGetShaderInfoLog(shader, len, NULL, log);
        fprintf(stderr, "[Compile error][%s]:\n%s\n", tag, log);
        delete[] log;
        return false;
    }
    return true;
}

bool ShaderProgram::link(GLuint program, const char* tag) {
    GLint ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (ok == GL_FALSE) {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        GLchar* log = new GLchar[len + 1];
        glGetProgramInfoLog(program, len, NULL, log);
        fprintf(stderr, "[Link error][%s]:\n%s\n", tag, log);
        delete[] log;
        return false;
    }
    return true;
}

void ShaderProgram::setUniform(const char* uniformName, const glm::mat4& matrix) const {
    GLint location = glGetUniformLocation(program_, uniformName);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void ShaderProgram::setUniform(const char* uniformName, float value) const {
    GLint location = glGetUniformLocation(program_, uniformName);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void ShaderProgram::setUniform(const char* uniformName, int value) const {
    GLint location = glGetUniformLocation(program_, uniformName);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void ShaderProgram::setUniform(const char* uniformName, const glm::vec2& vector) const {
    GLint location = glGetUniformLocation(program_, uniformName);
    if (location != -1) {
        glUniform2fv(location, 1, glm::value_ptr(vector));
    }
}

void ShaderProgram::setUniform(const char* uniformName, const glm::vec3& vector) const {
    GLint location = glGetUniformLocation(program_, uniformName);
    if (location != -1) {
        glUniform3fv(location, 1, glm::value_ptr(vector));
    }
}

void ShaderProgram::setUniform(const char* uniformName, const glm::vec4& vector) const {
    GLint location = glGetUniformLocation(program_, uniformName);
    if (location != -1) {
        glUniform4fv(location, 1, glm::value_ptr(vector));
    }
}
