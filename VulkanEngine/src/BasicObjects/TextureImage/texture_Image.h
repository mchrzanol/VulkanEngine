#pragma once
#include "Libraries.h"
#include "Renderer/Buffers.h"
#include "Initializers/initializers.h"

struct textureData {
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;

	uint32_t index;
};

class texturesLoading {
private:

public:
	VkSampler textureSampler;

	std::map<std::string, textureData> textures;

	textureData glitchedTexture;

	void createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, std::string name, std::string path);
	void createTextureImageView(VkDevice device, std::string name);
	void createTextureSampler(VkDevice device, VkPhysicalDevice physicalDevice);

	textureData createIndependentTexture(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, std::string path);

	void cleanup(VkDevice device);
private:
	void createImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void transitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};