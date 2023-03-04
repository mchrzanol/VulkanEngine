#include "window.h"

void windowClass::addHint(int hint, int value) {
    glfwWindowHint(hint, value);
}

void windowClass::initWindow(const char * WindowName, uint32_t width, uint32_t height, bool isResizable) {//const char * WindowName, uint32_t width = deafultHEIGHT, uint32_t height = deafultHEIGHT, bool isResizable = true
    glfwInit();

    m_Data.Title = WindowName;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    if(isResizable)
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(width, height, WindowName, nullptr, nullptr);
    
    if (m_window == NULL)
        throw std::runtime_error("failed to create window!");//maybe some log later

    if (isResizable) {
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    }
}

void windowClass::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<windowClass*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;

    glfwGetFramebufferSize(window, &app->m_Data.Width, &app->m_Data.Height);
}

void windowClass::main() {
    //uint32_t extensionCount = 0;
    //vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}

std::pair<int, int> windowClass::getCurrentSize() {
    framebufferResized = false;

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    return { width, height };
}

void windowClass::destroyWindow() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}