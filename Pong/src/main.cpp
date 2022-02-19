#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "display.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "shader.h"
#include "vertexBufferLayout.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main() {
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int height = 480, width = 640;
    Display display(height, width, "Pong");
    display.verifyWindow();

    if (glewInit() != GLEW_OK) {
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;


    float vertices[] = {
        400.0f, 100.0f,
        200.0f, 100.0f,
        300.0f,    300.0f
    };

    unsigned int indices[] = {
        0, 1, 2
    };

    VertexArray va;

    VertexBuffer vb(vertices, 3 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);

    va.addBuffer(vb, layout);
    
    IndexBuffer ib(indices, 3);

    glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    glm::mat4 mvp = proj * view * model;

    Shader shader("res/shaders/basic.shader");
    shader.bind();

    shader.setUniformMat4f("u_MVP", mvp);

    shader.unbind();
    va.unbind();
    vb.unbind();
    ib.unbind();

    Renderer renderer;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(display.getWindow())) {
        /* Render here */
        renderer.clear();
        shader.bind();

        // Draw food
        renderer.draw(va, ib, shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(display.getWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    return 0;
}