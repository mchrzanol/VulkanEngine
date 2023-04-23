#pragma once

#include "Libraries.h"

class ENGINE_API Input {
private:
    struct state {
        bool press = false;
        bool release = false;
    };

    struct cursorPos {
        double x = 0;
        double y = 0;
    };

    GLFWwindow* window;

    int* width;
    int* height;

    bool PrevCursorLockSet = true;

public:

    std::array<state, 349> keyState;

    cursorPos CurrentCursorPos;

    cursorPos deltaCursorPos;

    bool CursorLock = true;

    Input(GLFWwindow* window, int* width, int* height) {
        this->window = window;
        this->width = width;
        this->height = height;

        glfwSetKeyCallback(this->window, key_callback);
        //glfwSetCursorPosCallback(this->window, cursor_position_callback);

        glfwSetCursorPos(window, (double)*width / 2, (double)*height / 2);
    }

    void ClearKeyState() {
        for (auto& key : keyState) {            
            if (key.release) {
                key.release = false;
            }
        }

        deltaCursorPos = { 0,0 };
    }

    void MouseEvents() {

        if (CursorLock == true && PrevCursorLockSet == false) {
            glfwSetCursorPos(window, (double)*width / 2, (double)*height / 2);
            PrevCursorLockSet = true;
        }

        if (CursorLock == true) {
            glfwGetCursorPos(window, &CurrentCursorPos.x, &CurrentCursorPos.y);
            glfwSetCursorPos(window, (double)*width / 2, (double)*height / 2);

            deltaCursorPos.x = CurrentCursorPos.x - (double)*width / 2;
            deltaCursorPos.y = (double)*height / 2 - CurrentCursorPos.y;

            if (deltaCursorPos.y == 0.5)
                deltaCursorPos.y -= 0.5;
        }
        else
        {
            PrevCursorLockSet = CursorLock;
            deltaCursorPos = { 0,0 };
        }
    }

private:

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        void* user = glfwGetWindowUserPointer(window);
        Input* input = reinterpret_cast<Input*>(user);

        if (action == GLFW_PRESS) {
            input->keyState[key].press = true;
            input->keyState[key].release = false;
        }

        if (action == GLFW_RELEASE) {
            input->keyState[key].press = false;
            input->keyState[key].release = true;
        }
    }

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        void* user = glfwGetWindowUserPointer(window);
        Input* input = reinterpret_cast<Input*>(user);

        if (input->PrevCursorLockSet != input->CursorLock) {
            glfwSetCursorPos(window, (double)*input->width / 2, (double)*input->height / 2);
            input->PrevCursorLockSet = true;
        }
        
        if (input->CursorLock == true) {
            glfwGetCursorPos(window, &input->CurrentCursorPos.x, &input->CurrentCursorPos.y);
            glfwSetCursorPos(window, (double)*input->width / 2, (double)*input->height / 2);

            input->deltaCursorPos.x = input->CurrentCursorPos.x - (double)*input->width / 2;
            input->deltaCursorPos.y = (double)*input->height / 2 - input->CurrentCursorPos.y;
        } 
        else
        {
            input->PrevCursorLockSet = input->CursorLock;
            input->deltaCursorPos = { 0,0 };
        }
    }
};