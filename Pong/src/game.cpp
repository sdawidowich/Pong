#include <iostream>
#include <random>

#include "GL/glew.h"
#include "game.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Game* game;
    game = (Game*)glfwGetWindowUserPointer(window);

    if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && (action == GLFW_REPEAT || action == GLFW_PRESS))
        game->changeDirection(Direction::UP);
    else if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && (action == GLFW_REPEAT || action == GLFW_PRESS))
        game->changeDirection(Direction::DOWN);
    else if (action == GLFW_RELEASE)
        game->changeDirection(Direction::NONE);
}

Game::Game(const Display& p_display)
    : display(p_display), gameBegin(false), cpuPaddlePos{ 100, (double)p_display.getHeight() / 2 }, cpuPaddleDir(0), userPaddlePos{ (double)p_display.getWidth() - 100, (double)p_display.getHeight() / 2 }, userPaddleDir(0), ballDir{ 0, 0 }, ballPos{ (double)p_display.getWidth() / 2, ((double)p_display.getHeight() / 2) },
    proj(glm::ortho(0.0f, (float)display.getWidth(), 0.0f, (float)display.getHeight(), -1.0f, 1.0f)),
    view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))), model(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))) {


    glfwSetWindowUserPointer(display.getWindow(), this);
    glfwSetKeyCallback(display.getWindow(), key_callback);

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

    std::random_device random_device;
    std::mt19937 engine{ random_device() };

    int signs[2] = { 1, -1 };

    std::uniform_int_distribution<int> sign(0, 1);
    std::uniform_int_distribution<int> ySpeed(0, 3);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(display.getWindow())) {
        /* Render here */
        renderer.clear();
        shader.bind();

        if (cpuPaddlePos[1] + (paddleHeight / 2) < ballPos[1] + (ballWidth / 2) && (cpuPaddlePos[1] < (display.getHeight() - paddleHeight)))
            if (0.7 * abs(ballDir[0]) > 8) {
                cpuPaddlePos[1] += 8;
            }
            else
                cpuPaddlePos[1] += 0.7 * abs(ballDir[0]);
        else if (cpuPaddlePos[1] + (paddleHeight / 2) > ballPos[1] + (ballWidth / 2) && cpuPaddlePos[1] > 0)
            if (-0.7 * abs(ballDir[0]) < -8) {
                cpuPaddlePos[1] += -8;
            }
            else
                cpuPaddlePos[1] += -0.7 * abs(ballDir[0]);

        // Draw Cpu Paddle
        drawCpuPaddle(shader, renderer, vaPaddle, ib);

        if (userPaddlePos[1] > 0 && userPaddleDir == -1) 
            userPaddlePos[1] += userPaddleDir * 10;
        if (userPaddlePos[1] < (display.getHeight() - paddleHeight) && userPaddleDir == 1)
            userPaddlePos[1] += userPaddleDir * 10;
        // Draw User Paddle
        drawUserPaddle(shader, renderer, vaPaddle, ib);

        int ballIncrementX = ballDir[0];
        int ballIncrementY = ballDir[1];

        // If ball hits horizontal sides of screen, exit game and declare a winner
        if (ballPos[0] <= 0 && ballDir[0] < 0) {
            std::cout << "You win!" << std::endl;
            break;
        }
        if (ballPos[0] + ballWidth >= (double)display.getWidth() && ballDir[0] > 0) {
            std::cout << "You Lose!" << std::endl;
            break;
        }

        // If ball hits vertical sides of screen, reverse vertical direction to 'bounce' the ball
        if ((ballPos[1] <= 0 && ballDir[1] < 0) || (ballPos[1] + ballWidth >= (double)display.getHeight() && ballDir[1] > 0))
            ballDir[1] *= -1;

        // If ball hits either paddle, bounce ball off of it. Balls that hit center of paddle increase x speed, balls that hit top or bottom of paddle decrease x speed and increase y speed.
        if (ballPos[0] + ballWidth + ballIncrementX > userPaddlePos[0] && ballPos[0] + ballIncrementX < userPaddlePos[0] + paddleWidth
            && ballPos[1] + ballWidth + ballIncrementY > userPaddlePos[1] && ballPos[1] + ballIncrementY < userPaddlePos[1] + paddleHeight)
        {
            ballDir[0] *= -1;
            double hitPos = ((ballPos[1] + ballWidth / 2) - (userPaddlePos[1] + paddleHeight / 2)) / 25;
            double newXSpeed = ballDir[0] + (1 - abs(hitPos)) * ballDir[0] * 0.25;
            if (newXSpeed < -25)
                ballDir[0] = -25;
            else if (newXSpeed > -5)
                ballDir[0] = -5;
            else
                ballDir[0] = newXSpeed;

            if (abs(hitPos) > 2)
                ballDir[1] = (double)(signs[sign(engine)] * ySpeed(engine));
            else
                if (ballDir[1] < 1 && ballDir[1] > -1) {
                    ballDir[1] = (double)(signs[sign(engine)] * ySpeed(engine));
                }
                else
                    ballDir[1] = ballDir[1] + hitPos;
        }

        if (ballPos[0] + ballWidth + ballIncrementX > cpuPaddlePos[0] && ballPos[0] + ballIncrementX < cpuPaddlePos[0] + paddleWidth
            && ballPos[1] + ballWidth + ballIncrementY > cpuPaddlePos[1] && ballPos[1] + ballIncrementY < cpuPaddlePos[1] + paddleHeight) 
        {
            ballDir[0] *= -1;
            double hitPos = ((ballPos[1] + ballWidth / 2) - (cpuPaddlePos[1] + paddleHeight / 2)) / 25;
            double newXSpeed = ballDir[0] + (1 - abs(hitPos)) * ballDir[0] * 0.25;
            if (newXSpeed > 25)
                ballDir[0] = 25;
            else if (newXSpeed < 5)
                ballDir[0] = 5;
            else
                ballDir[0] = newXSpeed;

            if (abs(hitPos) > 2)
                ballDir[1] = (double)(signs[sign(engine)] * ySpeed(engine));
            else
                if (ballDir[1] < 1 && ballDir[1] > -1) {
                    ballDir[1] = (double)(signs[sign(engine)] * ySpeed(engine));
                }
                else
                    ballDir[1] = ballDir[1] + hitPos;
        }

        ballPos[0] += ballIncrementX;
        ballPos[1] += ballIncrementY;
        // Draw Ball
        drawBall(shader, renderer, vaBall, ib);

        /* Swap front and back buffers */
        glfwSwapBuffers(display.getWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void Game::beginGame() {
    gameBegin = true;
    std::random_device random_device;
    std::mt19937 engine{ random_device() };

    int signs[2] = { 1, -1 };

    std::uniform_int_distribution<int> sign(0, 1);
    std::uniform_int_distribution<int> xSpeed(4, 7);
    std::uniform_int_distribution<int> ySpeed(1, 4);

    ballDir[0] = (double)(signs[sign(engine)] * xSpeed(engine));
    ballDir[1] = (double)(signs[sign(engine)] * ySpeed(engine));
}

void Game::changeDirection(Direction direction) {
    if ((direction == Direction::UP || direction == Direction::DOWN) && !gameBegin)
        beginGame();

    if (direction == Direction::UP)
        userPaddleDir = 1;
    else if (direction == Direction::DOWN)
        userPaddleDir = -1;
    else if (direction == Direction::NONE)
        userPaddleDir = 0;
}

void Game::drawCpuPaddle(Shader& shader, Renderer& renderer, VertexArray& vaPaddle, IndexBuffer& ib) {
    model = glm::translate(glm::mat4(1.0f), glm::vec3(cpuPaddlePos[0], cpuPaddlePos[1], 0));
    mvp = proj * view * model;

    shader.setUniformMat4f("u_MVP", mvp);
    renderer.draw(vaPaddle, ib, shader);
}

void Game::drawUserPaddle(Shader& shader, Renderer& renderer, VertexArray& vaPaddle, IndexBuffer& ib) {
    model = glm::translate(glm::mat4(1.0f), glm::vec3(userPaddlePos[0], userPaddlePos[1], 0));
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
