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
void UniformBuffer::createUniform(unsigned int binding, size_t sizeofBindingValue, VkDescriptorType DescriptorType, TypeOfUniform UniformType) {
    unsigned int index = static_cast<int>(UniformType);

    bindingQueue[index].push_back(binding);

    UniformsCount[index]++;

    BindingData[index][binding] = { sizeofBindingValue, DescriptorType };

    uboLayoutBinding[index].resize(UniformsCount[index]);
    uboLayoutBinding[index][UniformsCount[index] -1].binding = binding;
    uboLayoutBinding[index][UniformsCount[index] - 1].descriptorType = DescriptorType;
    uboLayoutBinding[index][UniformsCount[index] - 1].descriptorCount = 1;

    uboLayoutBinding[index][UniformsCount[index] - 1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
}

void UniformBuffer::createUniformBuffers() {
    for (size_t frame = 1; frame < MAX_FRAMES_IN_FLIGHT + 1; frame++) {
        for (size_t index = 0; index < 2; index++) {
            for (auto& bind : bindingQueue[index]) {
                uniformBuffers[index].resize(UniformsCount[index] * frame);
                uniformBuffersMemory[index].resize(UniformsCount[index] * frame);
                uniformBuffersMapped[index].resize(UniformsCount[index] * frame);

                VkDeviceSize bufferSize;
                for (size_t i = (UniformsCount[index] - 1) * frame; i < UniformsCount[index] * frame; i++) {
                    bufferSize = BindingData[index][bind].offset;

                    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[index][i], uniformBuffersMemory[index][i], utils.GetDevice(), utils.GetPhyscicalDevice());

                    vkMapMemory(utils.GetDevice(), uniformBuffersMemory[index][i], 0, bufferSize, 0, &uniformBuffersMapped[index][i]);
                }

                VkDescriptorPoolSize poolSize{};
                poolSize.type = BindingData[index][bind].DescriptorType;
                poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

                this->poolSize[index].push_back(poolSize);
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

void UniformBuffer::updateUniformBuffer(uint32_t currentImage, VkExtent2D swapChainExtent) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    //ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.model = glm::mat4(1.f);

    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //ubo.view = glm::mat4(1.f);

    ubo.proj = glm::perspective(glm::radians(90.0f), (float)swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;
   // ubo.proj = glm::mat4(1.f);

    memcpy(uniformBuffersMapped[0][currentImage], &ubo, sizeof(ubo));
}

void UniformBuffer::createDescriptorSets() {

    for (size_t index = 0; index <2;index++)
    {
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayouts[index]);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool[index];
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets[index].resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(utils.GetDevice(), &allocInfo, descriptorSets[index].data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 1; i < MAX_FRAMES_IN_FLIGHT +1; i++) {//zrob jakis offset do tego max in flight bo jest 
            std::vector< VkWriteDescriptorSet> m_descriptorWrite(UniformsCount[index]);
            for (size_t bind = 0; bind < UniformsCount[index]; bind++) {
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = uniformBuffers[index][bind + bind*i];
                bufferInfo.offset = 0;
                bufferInfo.range = BindingData[index][bindingQueue[index][bind]].offset;

                VkWriteDescriptorSet descriptorWrite{};
                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = descriptorSets[index][i-1];
                descriptorWrite.dstBinding = bindingQueue[index][bind];
                descriptorWrite.dstArrayElement = 0;
                descriptorWrite.descriptorType = BindingData[index][bindingQueue[index][bind]].DescriptorType;
                descriptorWrite.descriptorCount = 1;
                descriptorWrite.pBufferInfo = &bufferInfo;
                descriptorWrite.pImageInfo = nullptr; // Optional
                descriptorWrite.pTexelBufferView = nullptr; // Optional

                m_descriptorWrite[bind] = descriptorWrite;


            }

            vkUpdateDescriptorSets(utils.GetDevice(), UniformsCount[index], m_descriptorWrite.data(), 0, nullptr);//tutaj mo¿esz daæ array tych descriptorów a pod countem iloœc bindów
        }
    }

}

void UniformBuffer::cleanup() {
    for (int index = 0; index < 2; index++) {
        for (size_t i = 0; i < uniformBuffers[index].size(); i++) {
            vkDestroyBuffer(utils.GetDevice(), uniformBuffers[index][i], nullptr);
            vkFreeMemory(utils.GetDevice(), uniformBuffersMemory[index][i], nullptr);
           // vkFreeMemory(utils.GetDevice(), uboLayoutBinding[index][i], nullptr);
        }

        vkDestroyDescriptorPool(utils.GetDevice(), descriptorPool[index], nullptr);

        vkDestroyDescriptorSetLayout(utils.GetDevice(), descriptorSetLayouts[index], nullptr);
    }

}