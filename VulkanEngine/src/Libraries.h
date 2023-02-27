#pragma once

#include "Core.h"						// The core of the dll


// add headers that you want to pre-compile here		////Check how to do precompiled files
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

//external
#define VK_USE_PLATFORM_WIN32_KHR
//#define GLFW_INCLUDE_VULKAN				// it replace #include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
//#include "backends/imgui_impl_vulkan.h"
//#include "backends/imgui_impl_glfw.h"
///#include "backends/imgui_impl_vulkan.cpp"
//#include "backends/imgui_impl_glfw.cpp"
#include <vulkan/vulkan.h>
//#include <glm/glm.hpp>					// do premake to do this stuff working or somehow do seprate projects for glfw, glm, vulkan and link them by references with engine or
//#include <glm/gtc/matrix_transform.hpp>	// you must convert those libs to static libs(they are dll now) to link them 

//C++ references
#include <iostream>

