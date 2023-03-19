#pragma once

#include "Libraries.h"

struct state {
    bool press = false;
    bool release = false;
    bool repeat = false;
};

class ENGINE_API Input {
public:

    std::array<state, 349> keyState;

    Input(GLFWwindow* window) {
        this->window = window;
        glfwSetKeyCallback(this->window, key_callback);
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        void* user = glfwGetWindowUserPointer(window);
        Input* input = reinterpret_cast<Input*>(user);

        if (action == GLFW_PRESS) {
            input->keyState[key].press = true;
        }
        else {
            input->keyState[key].press = false;
        }

        if (action == GLFW_RELEASE) {
            input->keyState[key].release = true;
        }
        else {
            input->keyState[key].release = false;
        }

        if (action == GLFW_PRESS) {
            input->keyState[key].repeat = true;
        }
        else {
            input->keyState[key].repeat = false;
        }
    }

private:
    GLFWwindow* window;
};