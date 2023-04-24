#pragma once

#include "Libraries.h"
#include "Renderer/Buffers.h"

class ENGINE_API depthBuffer {
public:
    void createDepthBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent3D swapChainExtent);

    void cleanup(VkDevice device);
private:
    VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
    bool hasStencilComponent(VkFormat format);
    void createImage(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent3D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
public:
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkFormat depthFormat;
};