#include "UniformBuffer.h"

void UniformBuffer::createDecriptorSetsLayout() {

    VkDescriptorSetLayoutCreateInfo GloballayoutInfo{};
    GloballayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    GloballayoutInfo.bindingCount = uboLayoutBinding[0].size();
    GloballayoutInfo.pBindings =  uboLayoutBinding[0].data();

    if (vkCreateDescriptorSetLayout(utils.GetDevice(), &GloballayoutInfo, nullptr, &descriptorSetLayouts[0]) != VK_SUCCESS) {
        throw std::runtime_error("failed to create Global descriptor set layout!");
    }

    VkDescriptorSetLayoutCreateInfo LocallayoutInfo{};
    LocallayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    LocallayoutInfo.bindingCount = uboLayoutBinding[1].size();
    LocallayoutInfo.pBindings = uboLayoutBinding[1].data();

    if (vkCreateDescriptorSetLayout(utils.GetDevice(), &LocallayoutInfo, nullptr, &descriptorSetLayouts[1]) != VK_SUCCESS) {
        throw std::runtime_error("failed to create Local descriptor set layout!");
    }
}
void UniformBuffer::createUniformBind(unsigned int binding, size_t sizeofBindingValue, VkDescriptorType DescriptorType, TypeOfUniform UniformType) {

    unsigned int set = static_cast<int>(UniformType);

    bindingQueue[set].push_back(binding);

    UniformsCount[set]++;

    BindingData[set][binding] = { sizeofBindingValue, DescriptorType };

    uboLayoutBinding[set].resize(UniformsCount[set]);
    uboLayoutBinding[set][UniformsCount[set] -1].binding = binding;
    uboLayoutBinding[set][UniformsCount[set] - 1].descriptorType = DescriptorType;
    uboLayoutBinding[set][UniformsCount[set] - 1].descriptorCount = 1;

    uboLayoutBinding[set][UniformsCount[set] - 1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
}

void UniformBuffer::createUniformBuffers() {
    for (size_t frame = 1; frame < MAX_FRAMES_IN_FLIGHT + 1; frame++) {
        for (size_t set = 0; set < 2; set++) {
            uniformBuffers[set].resize(UniformsCount[set] * frame);
            uniformBuffersMemory[set].resize(UniformsCount[set] * frame);
            uniformBuffersMapped[set].resize(UniformsCount[set] * frame);

            VkDeviceSize bufferSize;
            VkDescriptorPoolSize poolSize{};
            for (int bind = 0; bind < bindingQueue[set].size();bind++) {
                size_t i = (frame - 1) * UniformsCount[set];

                if (BindingData[set][bindingQueue[set][bind]].DescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
                    bufferSize = BindingData[set][bindingQueue[set][bind]].sizeofData;

                    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        uniformBuffers[set][i], uniformBuffersMemory[set][i], utils.GetDevice(), utils.GetPhyscicalDevice());
                }
                else {
                    bufferSize = GetAlignment(BindingData[set][bindingQueue[set][bind]].sizeofData) * MaximumObjectsOnFrame;
                    std::cout << bufferSize << std::endl;

                    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                        uniformBuffers[set][i], uniformBuffersMemory[set][i], utils.GetDevice(), utils.GetPhyscicalDevice());
                }

                vkMapMemory(utils.GetDevice(), uniformBuffersMemory[set][i], 0, bufferSize, 0, &uniformBuffersMapped[set][i]);

                poolSize.type = BindingData[set][bindingQueue[set][bind]].DescriptorType;
                poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

                this->poolSize[set].push_back(poolSize);

                i++;

            }
        }
    }
}

void UniformBuffer::createDescriptorPool() {

    VkDescriptorPoolCreateInfo global_poolInfo{};
    global_poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    global_poolInfo.poolSizeCount = UniformsCount[0];
    global_poolInfo.pPoolSizes = poolSize[0].data();

    global_poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);


    if (vkCreateDescriptorPool(utils.GetDevice(), &global_poolInfo, nullptr, &descriptorPool[0]) != VK_SUCCESS) {
        throw std::runtime_error("failed to create Global descriptor pool!");
    }

    VkDescriptorPoolCreateInfo local_poolInfo{};
    local_poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    local_poolInfo.poolSizeCount = UniformsCount[1];
    local_poolInfo.pPoolSizes = poolSize[1].data();

    local_poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);


    if (vkCreateDescriptorPool(utils.GetDevice(), &local_poolInfo, nullptr, &descriptorPool[1]) != VK_SUCCESS) {
        throw std::runtime_error("failed to create Local descriptor pool!");
    }
}

void UniformBuffer::createDescriptorSets() {

    for (size_t set = 0; set <2;set++)
    {
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayouts[set]);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool[set];
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets[set].resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(utils.GetDevice(), &allocInfo, descriptorSets[set].data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 1; i < MAX_FRAMES_IN_FLIGHT +1; i++) {
            std::vector< VkWriteDescriptorSet> m_descriptorWrite(UniformsCount[set]);

            for (size_t bind = 0; bind < UniformsCount[set]; bind++) {
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = uniformBuffers[set][bind + bind*(i-1)];
                bufferInfo.offset = 0;

                if (BindingData[set][bindingQueue[set][bind]].DescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
                    bufferInfo.range =  BindingData[set][bindingQueue[set][bind]].sizeofData;
                }
                else {
                    bufferInfo.range =  GetAlignment(BindingData[set][bindingQueue[set][bind]].sizeofData);
                }

                VkWriteDescriptorSet descriptorWrite{};
                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = descriptorSets[set][i-1];
                descriptorWrite.dstBinding = bindingQueue[set][bind];
                descriptorWrite.dstArrayElement = 0;
                descriptorWrite.descriptorType = BindingData[set][bindingQueue[set][bind]].DescriptorType;
                descriptorWrite.descriptorCount = 1;
                descriptorWrite.pBufferInfo = &bufferInfo;
                descriptorWrite.pImageInfo = nullptr; // Optional
                descriptorWrite.pTexelBufferView = nullptr; // Optional

                m_descriptorWrite[bind] = descriptorWrite;
            }

            vkUpdateDescriptorSets(utils.GetDevice(), UniformsCount[set], m_descriptorWrite.data(), 0, nullptr);
        }
    }

}


void UniformBuffer::cleanup() {
    for (int set = 0; set < 2; set++) {
        for (size_t i = 0; i < uniformBuffers[set].size(); i++) {
            vkDestroyBuffer(utils.GetDevice(), uniformBuffers[set][i], nullptr);
            vkFreeMemory(utils.GetDevice(), uniformBuffersMemory[set][i], nullptr);
        }

        vkDestroyDescriptorPool(utils.GetDevice(), descriptorPool[set], nullptr);

        vkDestroyDescriptorSetLayout(utils.GetDevice(), descriptorSetLayouts[set], nullptr);
    }

}