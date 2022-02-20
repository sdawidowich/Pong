#pragma once
#include "display.h"
#include "shader.h"
#include "renderer.h"
#include "vertexArray.h"
#include "indexBuffer.h"

#include "glm/glm.hpp"

class Game {
private:
	Display display;
	unsigned int cpuPaddlePos;
	unsigned int userPaddlePos;
	unsigned int ballPos[2];

	const float paddleVertices[8] = {
		 0.0f,   0.0f,
		15.0f,   0.0f,
		 0.0f, 100.0f,
		15.0f, 100.0f
	};
	const unsigned int indices[6] = {
		0, 1, 2,
		1, 2, 3
	};
	const float ballVertices[8]{
		 0.0f,  0.0f,
		20.0f,  0.0f,
		 0.0f, 20.0f,
		20.0f, 20.0f
	};

	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 model;
	glm::mat4 mvp;
public:
	Game(const Display& p_display);

	void drawCpuPaddle(Shader& shader, Renderer& renderer, VertexArray& vaPaddle, IndexBuffer& ib);
	void drawUserPaddle(Shader& shader, Renderer& renderer, VertexArray& vaPaddle, IndexBuffer& ib);
	void drawBall(Shader& shader, Renderer& renderer, VertexArray& vaBall, IndexBuffer& ib);
};