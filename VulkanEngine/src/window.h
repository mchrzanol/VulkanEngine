#pragma once
#include "Libraries.h"

uint32_t deafultWIDTH = 800;
uint32_t deafultHEIGHT = 600;

class ENGINE_API windowClass
{
public:
	GLFWwindow* window;
	
	void addHint(int hint, int value);//it must be called before initWindow

	void initWindow(const char * WindowName, uint32_t width = deafultHEIGHT, uint32_t height = deafultHEIGHT, bool isResizable = true);

	std::pair<int, int> getCurrentSize();

	void main();
private:
	bool framebufferResized = false;
private:
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};