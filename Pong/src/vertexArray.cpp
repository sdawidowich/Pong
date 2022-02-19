#include "vertexArray.h"
#include "renderer.h"
#include "vertexBufferLayout.h"

VertexArray::VertexArray() {
	glCall(glGenVertexArrays(1, &rendererID));
}

VertexArray::~VertexArray() {
	glCall(glDeleteVertexArrays(1, &rendererID));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    bind();
    vb.bind();
    const std::vector<VertexBufferElement>& elements = layout.getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const VertexBufferElement& element = elements[i];
        // Set layout of vertex positions and enable position attribute
        glCall(glEnableVertexAttribArray(i));
        glCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const {
    glCall(glBindVertexArray(rendererID));
}

void VertexArray::unbind() const {
    glCall(glBindVertexArray(0));
}
