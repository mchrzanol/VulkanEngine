#pragma once
#include "Libraries.h"

namespace vkinit {

	void create_descriptorSetLayout(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout, uint32_t bindingCount, const VkDescriptorSetLayoutBinding* pBindings);

	void create_descriptorPool(VkDevice device, VkDescriptorPool* descriptorPool, uint32_t poolSizeCount, const VkDescriptorPoolSize* pPoolSizes, uint32_t maxSets);

	VkWriteDescriptorSet write_descriptor_buffer(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorBufferInfo* bufferInfo,VkDescriptorImageInfo * imageInfo,  uint32_t binding, uint32_t descriptorCount);

	VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);
	void endSingleTimeCommands(VkDevice device, VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue);

	VkImageView createImageView(VkDevice device, VkImage image, VkFormat format);
}