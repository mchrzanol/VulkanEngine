#pragma once
#include "Libraries.h"

namespace vkinit {

	void create_descriptorSetLayout(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout, uint32_t bindingCount, const VkDescriptorSetLayoutBinding* pBindings);

	void create_descriptorPool(VkDevice device, VkDescriptorPool* descriptorPool, uint32_t poolSizeCount, const VkDescriptorPoolSize* pPoolSizes, uint32_t maxSets);

	VkWriteDescriptorSet write_descriptor_buffer(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorBufferInfo* bufferInfo, uint32_t binding);
}