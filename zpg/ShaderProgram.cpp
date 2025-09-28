#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram(const char* vertexSrc, const char* fragmentSrc)
{
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexSrc, nullptr);
    glCompileShader(vs);
    if (!compile(vs, "VERTEX")) {
        glDeleteShader(vs);
        program_ = 0;
        return;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentSrc, nullptr);
    glCompileShader(fs);
    if (!compile(fs, "FRAGMENT")) {
        glDeleteShader(vs);
        glDeleteShader(fs);
        program_ = 0;
        return;
    }

    program_ = glCreateProgram();
    glAttachShader(program_, vs);
    glAttachShader(program_, fs);
    glLinkProgram(program_);

    glDeleteShader(vs);
    glDeleteShader(fs);

    if (!link(program_, "PROGRAM")) {
        glDeleteProgram(program_);
        program_ = 0;
    }
}

ShaderProgram::~ShaderProgram() {
    if (program_) glDeleteProgram(program_);
}

void ShaderProgram::use() const {
    if (program_) glUseProgram(program_);
}

void ShaderProgram::unuse() const {
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
