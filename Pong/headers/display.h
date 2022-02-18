#pragma once

#include "GLFW/glfw3.h"

class Display {
private:
	GLFWwindow* window;
	int height, width;
	const char* windowName;
public:
	Display(int p_height, int p_width, const char* p_windowName);
	~Display();

	bool verifyWindow();
	GLFWwindow* getWindow() const;
};