#pragma once

#include "Libraries.h"
#include "GlobalVariables.h"

extern GlobalUtl utils;

class ENGINE_API descriptorSetLayout {
private:
	std::vector<VkDescriptorSetLayout> m_descriptorSetLayout;

public:
	descriptorSetLayout(unsigned int countOfSets, std::vector<unsigned int> BindingCount) {
        m_descriptorSetLayout.resize(countOfSets);

        for (auto & descriptor : m_descriptorSetLayout) {

            VkDescriptorSetLayoutBinding uboLayoutBinding{};

            uboLayoutBinding.binding = 0;
            uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uboLayoutBinding.descriptorCount = 1;

            uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            VkDescriptorSetLayoutCreateInfo GloballayoutInfo{};
            GloballayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            GloballayoutInfo.bindingCount = 1;
            GloballayoutInfo.pBindings = &uboLayoutBinding;

            if (vkCreateDescriptorSetLayout(utils.GetDevice(), &GloballayoutInfo, nullptr, &descriptor) != VK_SUCCESS) {
                throw std::runtime_error("failed to create Global descriptor set layout!");
            }
        }
	}
};
