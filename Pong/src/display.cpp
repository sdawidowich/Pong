#include "display.h"

#include <iostream>

Display::Display(int p_height, int p_width, const char* p_windowName) 
    : height(p_height), width(p_width), windowName(p_windowName) {
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(p_width, p_height, p_windowName, NULL, NULL);
}

Display::~Display() {
    glfwTerminate();
}

bool Display::verifyWindow() {
    if (!window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    return true;
}

GLFWwindow* Display::getWindow() const {
	return window;
}
