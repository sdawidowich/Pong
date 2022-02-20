#include "GL/glew.h"
#include "game.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Game::Game(const Display& p_display)
    : display(p_display), cpuPaddlePos(p_display.getHeight() / 2), userPaddlePos(p_display.getHeight() / 2), ballPos { p_display.getWidth() / 2, p_display.getHeight() / 2 },
    proj(glm::ortho(0.0f, (float)display.getWidth(), 0.0f, (float)display.getHeight(), -1.0f, 1.0f)),
    view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))), model(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))) {

    // Make paddle buffer
    VertexArray vaPaddle;
    VertexBuffer vbPaddle(paddleVertices, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    vaPaddle.addBuffer(vbPaddle, layout);

    // Make ball buffer
    VertexArray vaBall;
    VertexBuffer vbBall(ballVertices, 4 * 2 * sizeof(float));
    vaBall.addBuffer(vbBall, layout);

    IndexBuffer ib(indices, 6);


    mvp = proj * view * model;

    Renderer renderer;
    Shader shader("res/shaders/basic.shader");
    shader.bind();

    shader.setUniformMat4f("u_MVP", mvp);

    shader.unbind();
    vaPaddle.unbind();
    vaBall.unbind();
    vbPaddle.unbind();
    ib.unbind();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(display.getWindow())) {
        /* Render here */
        renderer.clear();
        shader.bind();

        // Draw Cpu Paddle
        drawCpuPaddle(shader, renderer, vaPaddle, ib);

        // Draw User Paddle
        drawUserPaddle(shader, renderer, vaPaddle, ib);

        // Draw Ball
        drawBall(shader, renderer, vaBall, ib);

        /* Swap front and back buffers */
        glfwSwapBuffers(display.getWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void Game::drawCpuPaddle(Shader& shader, Renderer& renderer, VertexArray& vaPaddle, IndexBuffer& ib) {
    model = glm::translate(glm::mat4(1.0f), glm::vec3(100, cpuPaddlePos, 0));
    mvp = proj * view * model;

    shader.setUniformMat4f("u_MVP", mvp);
    renderer.draw(vaPaddle, ib, shader);
}

void Game::drawUserPaddle(Shader& shader, Renderer& renderer, VertexArray& vaPaddle, IndexBuffer& ib) {
    model = glm::translate(glm::mat4(1.0f), glm::vec3(display.getWidth() - 100, userPaddlePos, 0));
    mvp = proj * view * model;

    shader.setUniformMat4f("u_MVP", mvp);
    renderer.draw(vaPaddle, ib, shader);
}

void Game::drawBall(Shader& shader, Renderer& renderer, VertexArray& vaBall, IndexBuffer& ib) {
    model = glm::translate(glm::mat4(1.0f), glm::vec3(ballPos[0], ballPos[1], 0));
    mvp = proj * view * model;

    shader.setUniformMat4f("u_MVP", mvp);
    renderer.draw(vaBall, ib, shader);
}
