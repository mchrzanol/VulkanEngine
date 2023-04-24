#pragma once

#include "Libraries.h"
#include "Renderer/Buffers.h"

class ENGINE_API depthBuffer {
public:
    void createDepthBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent3D swapChainExtent);

    VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

    void cleanup(VkDevice device);
private:
    VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    bool hasStencilComponent(VkFormat format);
    void createImage(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent3D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
public:
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
};