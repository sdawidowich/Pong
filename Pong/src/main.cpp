#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "display.h"

int main() {

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    Display display(480, 640, "Pong");
    display.verifyWindow();

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(display.getWindow())) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(display.getWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    return 0;
}