#pragma once

#include "Core.h"						// The core of the dll


// add headers that you want to pre-compile here		////Check how to do precompiled files
#define WIN32_LEAN_AND_MEAN					// Exclude rarely-used stuff from Windows headers
#include <windows.h>

//external
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

//C++ references
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <set>
#include <fstream>
#include <array>
#include <algorithm>
#include <chrono>
