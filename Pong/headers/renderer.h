#pragma once
#include <GL/glew.h>

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) glClearErrors();\
    x;\
    ASSERT(glLogCall(#x, __FILE__, __LINE__))

void glClearErrors();
bool glLogCall(const char* function, const char* file, int line);

class Renderer {
public:
    void clear() const;
    void draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
};