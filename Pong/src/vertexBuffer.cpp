#include "renderer.h"
#include "vertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	glCall(glDeleteBuffers(1, &rendererID));
}

void VertexBuffer::bind() const {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::unbind() const {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
