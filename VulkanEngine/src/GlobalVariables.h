#pragma once
#include "Libraries.h"

class GlobalUtl {

protected:
	VkDevice device;
	VkCommandPool commandPool;
	VkQueue graphicsQueue;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	uint32_t MaximumObjectsOnFrame;

public:
	void BindDevice(VkDevice  device) { this->device = device; };
	VkDevice GetDevice() { return device; };

	void BindCommandPool(VkCommandPool commandPool) { this->commandPool = commandPool; };
	VkCommandPool GetCommandPool() { return commandPool; };

	void BindGraphicsQueue(VkQueue graphicsQueue) { this->graphicsQueue = graphicsQueue; };
	VkQueue GetGraphicsQueue() { return graphicsQueue; };

	void BindPhysicalDevice(VkPhysicalDevice physicalDevice) { this->physicalDevice = physicalDevice; };
	VkPhysicalDevice GetPhyscicalDevice() { return physicalDevice; };

	void GiveMaximumObjectsOnFrame(uint32_t physicalDevice) { this->MaximumObjectsOnFrame = MaximumObjectsOnFrame; };
	uint32_t GetMaximumObjectsOnFrame() { return MaximumObjectsOnFrame; };

};