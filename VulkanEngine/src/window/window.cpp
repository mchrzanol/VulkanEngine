#include "window.h"

void windowClass::addHint(int hint, int value) {
    glfwWindowHint(hint, value);
}

void windowClass::initWindow(const char * WindowName, uint32_t width, uint32_t height, bool isResizable) {//const char * WindowName, uint32_t width = deafultHEIGHT, uint32_t height = deafultHEIGHT, bool isResizable = true
    glfwInit();

    m_Data.Title = WindowName;
    m_Data.Width = width;
    m_Data.Height = height;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    if(isResizable)
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    else
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(width, height, WindowName, nullptr, nullptr);
    
    if (m_window == NULL)
        throw std::runtime_error("failed to create window!");//maybe some log later

    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);

    lastTime = glfwGetTime();
}

void windowClass::addUserPointer(Input* input) {
    this->input = input;

    glfwSetWindowUserPointer(m_window, this->input);
}

void windowClass::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<windowClass*>(glfwGetWindowUserPointer(window));
    app->m_Data.framebufferResized = true;

    glfwGetFramebufferSize(window, &app->m_Data.Width, &app->m_Data.Height);
}

void windowClass::UpdateWindowSize() {
    m_Data.framebufferResized = false;

    glfwGetFramebufferSize(m_window, &m_Data.Width, &m_Data.Height);
}

void windowClass::setWindowFPS()
{
    // Measure speed
    double currentTime = glfwGetTime();
    Frames++;
    if (currentTime - lastTime >= 1.0) { // If last cout was more than 1 sec ago

        std::string title = m_Data.Title;
        title += " [FPS:";
        title += std::to_string(Frames);
        title += "]";

        glfwSetWindowTitle(m_window, title.c_str());

        Frames = 0;
        lastTime += 1.0;
    }
}

void windowClass::destroyWindow() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}