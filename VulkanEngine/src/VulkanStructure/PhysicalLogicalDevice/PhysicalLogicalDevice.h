#pragma once

#include "Libraries.h"
#include "VulkanStructure/Structures.h"


class ENGINE_API PickingDevice {
private:
	const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
public:
	void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

	void createLogicalDevice(std::vector<const char*> validationLayers, VkSurfaceKHR surface, VkDevice& device);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
private:

	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, SwapChainSupportDetails swapChainSupport);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
public:
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	VkQueue graphicsQueue;
	VkQueue presentQueue;
};