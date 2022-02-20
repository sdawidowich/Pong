#pragma once

#include "GLFW/glfw3.h"

class Display {
private:
	GLFWwindow* window;
	unsigned int height, width;
	const char* windowName;
public:
	Display(int p_height, int p_width, const char* p_windowName);
	~Display();

	bool verifyWindow();
	GLFWwindow* getWindow() const;
	inline unsigned int getHeight() const { return height; };
	inline unsigned int getWidth() const { return width; };
};