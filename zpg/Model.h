#pragma once

#include <GL/glew.h>
#include <cstddef> 

class Model {
public:
	Model();
	~Model();

	void upload(const void* vertices, GLsizeiptr sizeBytes, GLsizei strideBytes, GLenum usage = GL_STATIC_DRAW);

	void enableAttrib(GLuint index, GLint size, GLenum type,
		GLboolean normalized, GLsizei stride, size_t offsetBytes);

	void draw(GLenum mode, GLint first, GLsizei count) const;
	GLsizei getVertexCount() const { return vertexCount_; }

private:
	GLuint vao_ = 0;
	GLuint vbo_ = 0;

	GLsizei vertexCount_ = 0;
};
