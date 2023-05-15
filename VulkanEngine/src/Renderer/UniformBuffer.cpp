#include "UniformBuffer.h"

void UniformBuffer::createDecriptorSetsLayout(VkDevice device) {

    //GlobalLayout
    vkinit::create_descriptorSetLayout(device, &descriptorSetLayouts[0], uboLayoutBinding[0].size(), uboLayoutBinding[0].data());

    //LocalLayout/Entity
    vkinit::create_descriptorSetLayout(device, &descriptorSetLayouts[1], uboLayoutBinding[1].size(), uboLayoutBinding[1].data());
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

    //to do ogarniecia pozniej jak sie zacznie wywalac/jak mi sie zachce
}

void UniformBuffer::createUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice) {
    uniformBuffers[0].resize(2);
    uniformBuffersMemory[0].resize(2);
    uniformBuffersMapped[0].resize(2);

    uniformBuffers[1].resize(2);
    uniformBuffersMemory[1].resize(2);
    uniformBuffersMapped[1].resize(2);

    for (size_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT ; frame++) {

        //set 0
        VkDeviceSize bufferSize;
        VkDescriptorPoolSize poolSize{};

        bufferSize = BindingData[0][0].sizeofData;
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniformBuffers[0][frame], uniformBuffersMemory[0][frame], device, physicalDevice);

        vkMapMemory(device, uniformBuffersMemory[0][frame], 0, bufferSize, 0, &uniformBuffersMapped[0][frame]);

        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        this->poolSize[0].push_back(poolSize);


        //set 1
        bufferSize = BindingData[1][0].sizeofData;
        createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniformBuffers[1][frame], uniformBuffersMemory[1][frame], device, physicalDevice);

        vkMapMemory(device, uniformBuffersMemory[1][frame], 0, bufferSize, 0, &uniformBuffersMapped[1][frame]);

        poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        this->poolSize[1].push_back(poolSize);
    }
}

void UniformBuffer::createDescriptorPool(VkDevice device) {

    //GlobalPool
    vkinit::create_descriptorPool(device, &descriptorPool[0], UniformsCount[0], poolSize[0].data(), MAX_FRAMES_IN_FLIGHT);

    //LocalPool/Entity
    vkinit::create_descriptorPool(device, &descriptorPool[1], UniformsCount[1], poolSize[1].data(), MAX_FRAMES_IN_FLIGHT);

}

void UniformBuffer::createDescriptorSets(VkDevice device, VkPhysicalDevice physicalDevice) {
    VkDescriptorSetAllocateInfo allocInfo{};

    //set 0
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayouts[0]);
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool[0];
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets[0].resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets[0].data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT; frame++) {
            std::vector< VkWriteDescriptorSet> m_descriptorWrite(1);
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = uniformBuffers[0][frame];
                bufferInfo.offset = 0;
                bufferInfo.range = BindingData[0][0].sizeofData;;

                m_descriptorWrite[0] = vkinit::write_descriptor_buffer(BindingData[0][0].DescriptorType, descriptorSets[0][frame], &bufferInfo, 0);
            vkUpdateDescriptorSets(device, 1, m_descriptorWrite.data(), 0, nullptr);
        }

    //set 1
        std::vector<VkDescriptorSetLayout> layouts2(MAX_FRAMES_IN_FLIGHT, descriptorSetLayouts[1]);
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool[1];
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts2.data();

        descriptorSets[1].resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets[1].data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT; frame++) {
            std::vector< VkWriteDescriptorSet> m_descriptorWrite(1);
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[1][frame];
            bufferInfo.offset = 0;
            bufferInfo.range = BindingData[1][0].sizeofData;

            m_descriptorWrite[0] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, descriptorSets[1][frame], &bufferInfo, 0);
            vkUpdateDescriptorSets(device, 1, m_descriptorWrite.data(), 0, nullptr);
        }

}


void UniformBuffer::cleanup(VkDevice device) {
    for (int set = 0; set < 2; set++) {
        for (size_t i = 0; i < uniformBuffers[set].size(); i++) {
            vkDestroyBuffer(device, uniformBuffers[set][i], nullptr);
            vkFreeMemory(device, uniformBuffersMemory[set][i], nullptr);
        }

        vkDestroyDescriptorPool(device, descriptorPool[set], nullptr);

        vkDestroyDescriptorSetLayout(device, descriptorSetLayouts[set], nullptr);
    }

}