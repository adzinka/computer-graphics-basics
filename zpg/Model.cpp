#include "Model.h"

Model::Model() {
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
}

Model::~Model() {
	if (vbo_) glDeleteBuffers(1, &vbo_);
	if (vao_) glDeleteVertexArrays(1, &vao_);
}

void Model::upload(const void* vertices, GLsizeiptr sizeBytes, GLsizei strideBytes, GLenum usage) {
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeBytes, vertices, usage);

	if (strideBytes > 0) {
		vertexCount_ = sizeBytes / strideBytes;
	}
}

void Model::enableAttrib(
	GLuint index,
	GLint size,
	GLenum type,
	GLboolean normalized,
	GLsizei stride,
	size_t offsetBytes
) {
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride,
		reinterpret_cast<const void*>(offsetBytes));
}

void Model::draw(GLenum mode, GLint first, GLsizei count) const {
	glBindVertexArray(vao_);
	glDrawArrays(mode, first, count);
}
