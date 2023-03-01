#include <iostream>
#include "Engine.h"

class TestPlatform : public App {
public:
	virtual void Print() {
		windowClass* window = new windowClass;
		window->initWindow("TestPlatform");
		std::cout << "HelloWorld";
		window->main();
	}
};
int main()
{
	//App * a = new App;
	//a->Print();
	TestPlatform* a = new TestPlatform;
	a->Print();

	//GLFWwindow* window;
	//window = glfwCreateWindow(100, 2000, " ", nullptr, nullptr);
	//glfwInit();
	//windowClass* window = new windowClass;
	//uint32_t extensionCount = 0;
	//vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	//window->initWindow("TestPlatform");
	//window->main();
	//while (!glfwWindowShouldClose(window->window)) {
	//	glfwPollEvents();
	//}
	return 0;
}