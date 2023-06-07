#pragma once

#include "Libraries.h"

class ENGINE_API frameBuffer {
public:
	void createFramebuffer(VkDevice device, std::vector<VkImageView> swapChainImageViews, VkImageView depthImageView, VkExtent3D swapChainExtent, VkRenderPass renderPass);

	void cleanup(VkDevice device);

public:
	std::vector<VkFramebuffer> swapChainFramebuffers;
};