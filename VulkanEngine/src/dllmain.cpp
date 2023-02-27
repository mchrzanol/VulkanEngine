#include "dllmain.h"

void App::Print() {
	glfwInit();
	GLFWwindow* window;
	window = glfwCreateWindow(100, 100, "Vulkan", nullptr, nullptr);

	glm::mat4 a = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	std::cout << "HelloWorld";
}