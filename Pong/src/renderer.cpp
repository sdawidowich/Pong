#include <iostream>
#include "renderer.h"

void glClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const {
    shader.bind();
    vao.bind();
    ibo.bind();

    glCall(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
}
