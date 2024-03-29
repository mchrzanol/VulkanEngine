#pragma once

#include "Core.h"						// The core of the dll


// add headers that you want to pre-compile here		////Check how to do precompiled files
#define WIN32_LEAN_AND_MEAN					// Exclude rarely-used stuff from Windows headers
#include <windows.h>

//external
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ON
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <imgui.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

#include <tiny_obj_loader.h>

#include <CppCoreCheck\Warnings.h>

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
#include <functional>
#include <stdlib.h>
#include <map>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

