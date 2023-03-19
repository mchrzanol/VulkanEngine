#pragma once

#include "Libraries.h"

struct state {
    bool press = false;
    bool release = false;
};

class ENGINE_API Input {
public:

    std::array<state, 349> keyState;

    Input(GLFWwindow* window) {
        this->window = window;
        glfwSetKeyCallback(this->window, key_callback);
    }

    void ClearKeyState() {
        for (auto& key : keyState) {
            if (key.press) {
                key.press = false;
            }
            
            if (key.release) {
                key.release = false;
            }
        }
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        void* user = glfwGetWindowUserPointer(window);
        Input* input = reinterpret_cast<Input*>(user);

        if (action == GLFW_PRESS) {
            input->keyState[key].press = true;
        }

        if (action == GLFW_RELEASE) {
            input->keyState[key].release = true;
        }
    }

private:

    GLFWwindow* window;
};