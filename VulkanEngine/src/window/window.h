#pragma once
#include "Libraries.h"

const uint32_t deafultWIDTH = 800;
const uint32_t deafultHEIGHT = 600;

struct WindowData 
{
	std::string Title;
	int Width = deafultWIDTH, Height = deafultHEIGHT;
	bool framebufferResized = false;
};

class ENGINE_API windowClass
{
public:
	GLFWwindow* m_window;

	WindowData m_Data;
	
	void addHint(int hint, int value);//it must be called before initWindow

	void initWindow(const char * WindowName, uint32_t width = deafultHEIGHT, uint32_t height = deafultHEIGHT, bool isResizable = true);

	void UpdateWindowSize();

	void destroyWindow();

	bool isWindowClosed() { return glfwWindowShouldClose(m_window); };
	void PoolEvents() { glfwPollEvents(); };
private:
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};