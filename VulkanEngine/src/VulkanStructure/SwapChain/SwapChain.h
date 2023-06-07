#pragma once

#include "Libraries.h"
#include "VulkanStructure/Structures.h"

class ENGINE_API SwapChain {
public:
    void createSwapChain(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkDevice device, GLFWwindow* window, QueueFamilyIndices indices);
    void createImageViews(VkDevice device);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    void cleanupSwapChain(VkDevice device);
private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent3D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow * window);
public:
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent3D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
};