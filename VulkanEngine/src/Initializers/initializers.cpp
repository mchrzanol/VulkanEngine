#include "initializers.h"

void vkinit::create_descriptorSetLayout(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout, uint32_t bindingCount, const VkDescriptorSetLayoutBinding* pBindings) {
     VkDescriptorSetLayoutCreateInfo layoutInfo{};
     layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
     layoutInfo.bindingCount = bindingCount;
     layoutInfo.pBindings = pBindings;

     if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, descriptorSetLayout) != VK_SUCCESS) {
         throw std::runtime_error("failed to create Global descriptor set layout!");
    }
}

void vkinit::create_descriptorPool(VkDevice device, VkDescriptorPool* descriptorPool, uint32_t poolSizeCount, const VkDescriptorPoolSize* pPoolSizes, uint32_t maxSets) {
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizeCount;
    poolInfo.pPoolSizes = pPoolSizes;

    poolInfo.maxSets = maxSets;


    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create Global descriptor pool!");
    }
}

VkWriteDescriptorSet vkinit::write_descriptor_buffer(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorBufferInfo* bufferInfo, uint32_t binding)
{
    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;

    write.dstBinding = binding;
    write.dstSet = dstSet;
    write.descriptorCount = 1;
    write.descriptorType = type;
    write.pBufferInfo = bufferInfo;

    return write;
}