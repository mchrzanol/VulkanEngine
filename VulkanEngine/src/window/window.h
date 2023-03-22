#pragma once
#include "Libraries.h"
#include "Input/Input.h"

#pragma warning(disable: 26495)

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

	void initWindow(const char * WindowName, uint32_t width = deafultWIDTH, uint32_t height = deafultHEIGHT, bool isResizable = true);

	void addUserPointer(Input* input);

	void UpdateWindowSize();

	void setWindowFPS();

	void destroyWindow();

	bool isWindowClosed() { return glfwWindowShouldClose(m_window); };
	void PoolEvents() { glfwPollEvents(); };
private:
	Input* input;

	int Frames = 0;
	float lastTime = 0;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};