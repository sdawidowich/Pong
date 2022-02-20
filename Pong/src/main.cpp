#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "display.h"
#include "game.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main() {
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int height = 720, width = 1080;
    Display display(height, width, "Pong");
    display.verifyWindow();

    if (glewInit() != GLEW_OK) {
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    Game game(display);

    return 0;
}