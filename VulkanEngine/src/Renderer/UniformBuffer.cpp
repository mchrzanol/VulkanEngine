#include "UniformBuffer.h"

void UniformBuffer::createBasicDescriptorSetsLayout(VkDevice device) {
    //GlobalLayout
    uboLayoutBinding[0].resize(1);
    uboLayoutBinding[0][0].binding = 0;
    uboLayoutBinding[0][0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding[0][0].descriptorCount = 1;

    uboLayoutBinding[0][0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    vkinit::create_descriptorSetLayout(device, &descriptorSetLayouts[0], uboLayoutBinding[0].size(), uboLayoutBinding[0].data());

    //LocalLayout/Entity
    uboLayoutBinding[1].resize(1);
    uboLayoutBinding[1][0].binding = 0;
    uboLayoutBinding[1][0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    uboLayoutBinding[1][0].descriptorCount = 1;

    uboLayoutBinding[1][0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    //Test-Textures
    uboLayoutBinding[1].resize(2);
    uboLayoutBinding[1][1].binding = 1;
    uboLayoutBinding[1][1].descriptorCount = MaximumTextures + 1;
    uboLayoutBinding[1][1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    uboLayoutBinding[1][1].pImmutableSamplers = nullptr;

    uboLayoutBinding[1][1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    //sampler
    uboLayoutBinding[1].resize(3);
    uboLayoutBinding[1][2].binding = 2;
    uboLayoutBinding[1][2].descriptorCount = 1;
    uboLayoutBinding[1][2].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    uboLayoutBinding[1][2].pImmutableSamplers = nullptr;

    uboLayoutBinding[1][2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    //textureIndexing
    uboLayoutBinding[1].resize(4);
    uboLayoutBinding[1][3].binding = 3;
    uboLayoutBinding[1][3].descriptorCount = 1;
    uboLayoutBinding[1][3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    uboLayoutBinding[1][3].pImmutableSamplers = nullptr;

    uboLayoutBinding[1][3].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    vkinit::create_descriptorSetLayout(device, &descriptorSetLayouts[1], uboLayoutBinding[1].size(), uboLayoutBinding[1].data());
}

void UniformBuffer::createModelDescriptorSetsLayout(VkDevice device) {

    //GlobalLayout
        //UniformBufferObject
    model_uboLayoutBinding[0].resize(1);
    model_uboLayoutBinding[0][0].binding = 0;
    model_uboLayoutBinding[0][0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    model_uboLayoutBinding[0][0].descriptorCount = 1;

    model_uboLayoutBinding[0][0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    vkinit::create_descriptorSetLayout(device, &model_descriptorSetLayouts[0], model_uboLayoutBinding[0].size(), model_uboLayoutBinding[0].data());

    //LocalLayout/Entity

        //modelUBO
    model_uboLayoutBinding[1].resize(1);
    model_uboLayoutBinding[1][0].binding = 0;
    model_uboLayoutBinding[1][0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    model_uboLayoutBinding[1][0].descriptorCount = 1;

    model_uboLayoutBinding[1][0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        //texture + Sampler
    model_uboLayoutBinding[1].resize(2);
    model_uboLayoutBinding[1][1].binding = 1;
    model_uboLayoutBinding[1][1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    model_uboLayoutBinding[1][1].descriptorCount = 1;

    model_uboLayoutBinding[1][1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    vkinit::create_descriptorSetLayout(device, &model_descriptorSetLayouts[1], model_uboLayoutBinding[1].size(), model_uboLayoutBinding[1].data());
}

void UniformBuffer::createDecriptorSetsLayout(VkDevice device) {

    createBasicDescriptorSetsLayout(device);
    createModelDescriptorSetsLayout(device);

}

void UniformBuffer::createBasicUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice) {
    uniformBuffers[0].resize(2);
    uniformBuffersMemory[0].resize(2);
    uniformBuffersMapped[0].resize(2);

    uniformBuffers[1].resize(4);
    uniformBuffersMemory[1].resize(4);
    uniformBuffersMapped[1].resize(4);

    for (size_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT; frame++) {

        //set 0
        VkDeviceSize bufferSize;
        VkDescriptorPoolSize poolSize{};

        bufferSize = sizeof(UniformBufferObject);
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniformBuffers[0][frame], uniformBuffersMemory[0][frame], device, physicalDevice);

        vkMapMemory(device, uniformBuffersMemory[0][frame], 0, bufferSize, 0, &uniformBuffersMapped[0][frame]);

        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        this->poolSize[0].push_back(poolSize);

        //set 1

        //modelUBO
        bufferSize = GetAlignment(sizeof(modelUBO), physicalDevice) * MaximumObjectsOnFrame;
        createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniformBuffers[1][frame], uniformBuffersMemory[1][frame], device, physicalDevice);

        vkMapMemory(device, uniformBuffersMemory[1][frame], 0, bufferSize, 0, &uniformBuffersMapped[1][frame]);

        poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        this->poolSize[1].push_back(poolSize);

        //Test-testures

        poolSize.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        this->poolSize[1].push_back(poolSize);

        //sampler
        poolSize.type = VK_DESCRIPTOR_TYPE_SAMPLER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        //textureIndexing
        bufferSize = GetAlignment(sizeof(int), physicalDevice) * MaximumObjectsOnFrame;
        createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniformBuffers[1][frame + 2], uniformBuffersMemory[1][frame + 2], device, physicalDevice);

        vkMapMemory(device, uniformBuffersMemory[1][frame + 2], 0, bufferSize, 0, &uniformBuffersMapped[1][frame + 2]);

        poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        this->poolSize[1].push_back(poolSize);

        this->poolSize[1].push_back(poolSize);
    }
}

void UniformBuffer::createModelUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice) {
    model_uniformBuffers[0].resize(2);
    model_uniformBuffersMemory[0].resize(2);
    model_uniformBuffersMapped[0].resize(2);

    model_uniformBuffers[1].resize(4);
    model_uniformBuffersMemory[1].resize(4);
    model_uniformBuffersMapped[1].resize(4);

    for (size_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT; frame++) {

        //set 0
        VkDeviceSize bufferSize;
        VkDescriptorPoolSize poolSize{};

        bufferSize = sizeof(UniformBufferObject);
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            model_uniformBuffers[0][frame], model_uniformBuffersMemory[0][frame], device, physicalDevice);

        vkMapMemory(device, model_uniformBuffersMemory[0][frame], 0, bufferSize, 0, &model_uniformBuffersMapped[0][frame]);

        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        this->model_poolSize[0].push_back(poolSize);

        //set 1

        //modelUBO
        bufferSize = GetAlignment(sizeof(modelUBO), physicalDevice) * MaximumObjectsOnFrame;
        createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            model_uniformBuffers[1][frame], model_uniformBuffersMemory[1][frame], device, physicalDevice);

        vkMapMemory(device, model_uniformBuffersMemory[1][frame], 0, bufferSize, 0, &model_uniformBuffersMapped[1][frame]);

        poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        this->model_poolSize[1].push_back(poolSize);

        //texture + sampler

        poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        this->model_poolSize[1].push_back(poolSize);
    }
}

void UniformBuffer::createUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice) {
    createBasicUniformBuffers(device, physicalDevice);

    createModelUniformBuffers(device, physicalDevice);

}

void UniformBuffer::createDescriptorPool(VkDevice device) {

    //Basic
    //GlobalPool
    vkinit::create_descriptorPool(device, &descriptorPool[0], 1, poolSize[0].data(), MAX_FRAMES_IN_FLIGHT);

    //LocalPool/Entity
    vkinit::create_descriptorPool(device, &descriptorPool[1], 1, poolSize[1].data(), MAX_FRAMES_IN_FLIGHT);


    //Model
    //GlobalPool
    vkinit::create_descriptorPool(device, &model_descriptorPool[0], 1, model_poolSize[0].data(), MAX_FRAMES_IN_FLIGHT);

    //LocalPool/Entity
    vkinit::create_descriptorPool(device, &model_descriptorPool[1], 1, model_poolSize[1].data(), MAX_FRAMES_IN_FLIGHT);

}

void UniformBuffer::createBasicDescriptorSets(VkDevice device, VkPhysicalDevice physicalDevice, VkSampler sampler, std::map<std::string, textureData> & textures, textureData & glitched) {
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
                bufferInfo.range = sizeof(UniformBufferObject);

                m_descriptorWrite[0] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorSets[0][frame], &bufferInfo,0, 0, 1);
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
            std::vector< VkWriteDescriptorSet> m_descriptorWriteSet1(4);

            //modelUBO
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[1][frame];
            bufferInfo.offset = 0;
            bufferInfo.range = GetAlignment(sizeof(modelUBO), physicalDevice) *MaximumObjectsOnFrame;

            m_descriptorWriteSet1[0] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, descriptorSets[1][frame], &bufferInfo, 0, 0, 1);

            //Textures

            std::vector<VkDescriptorImageInfo> imageInfo;

            imageInfo.push_back(VkDescriptorImageInfo{});
            imageInfo.back().imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.back().imageView = glitched.textureImageView;
            imageInfo.back().sampler = nullptr;

            uint32_t textureIndex = 1;//index 0 is reserved for invalid texture
            for (auto& texture : textures) {
                imageInfo.push_back(VkDescriptorImageInfo{});
                imageInfo.back().imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.back().imageView = texture.second.textureImageView;
                imageInfo.back().sampler = nullptr;

                texture.second.index = textureIndex;

                textureIndex++;
            }

            imageInfo.resize(MaximumTextures+1);
            for (int i = textures.size()+1; i < MaximumTextures+1; i++) {
                imageInfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo[i].imageView = glitched.textureImageView;
                imageInfo[i].sampler = nullptr;
            }

            m_descriptorWriteSet1[1] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, descriptorSets[1][frame],0, imageInfo.data(), 1, MaximumTextures+1);
           
            //sampler
            VkDescriptorImageInfo samplerInfo;
            samplerInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            samplerInfo.imageView = nullptr;
            samplerInfo.sampler = sampler;

            m_descriptorWriteSet1[2] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_SAMPLER, descriptorSets[1][frame], 0, &samplerInfo, 2, 1);

            //textureIndexing
            VkDescriptorBufferInfo bufferInfo2{};
            bufferInfo2.buffer = uniformBuffers[1][frame+2];
            bufferInfo2.offset = 0;
            bufferInfo2.range = GetAlignment(sizeof(int), physicalDevice) * MaximumObjectsOnFrame;

            m_descriptorWriteSet1[3] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, descriptorSets[1][frame], &bufferInfo2, 0, 3, 1);


            vkUpdateDescriptorSets(device, 4, m_descriptorWriteSet1.data(), 0, nullptr);
        }

}

void UniformBuffer::createModelDescriptorSets(VkDevice device, VkPhysicalDevice physicalDevice, VkSampler sampler, textureData& texture) {
    VkDescriptorSetAllocateInfo allocInfo{};

    //set 0
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, model_descriptorSetLayouts[0]);
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = model_descriptorPool[0];
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    model_descriptorSets[0].resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(device, &allocInfo, model_descriptorSets[0].data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT; frame++) {
        std::vector< VkWriteDescriptorSet> m_descriptorWrite(1);
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = model_uniformBuffers[0][frame];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        m_descriptorWrite[0] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, model_descriptorSets[0][frame], &bufferInfo, 0, 0, 1);
        vkUpdateDescriptorSets(device, 1, m_descriptorWrite.data(), 0, nullptr);
    }

    //set 1
    std::vector<VkDescriptorSetLayout> layouts2(MAX_FRAMES_IN_FLIGHT, model_descriptorSetLayouts[1]);
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = model_descriptorPool[1];
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts2.data();

    model_descriptorSets[1].resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(device, &allocInfo, model_descriptorSets[1].data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT; frame++) {
        std::vector< VkWriteDescriptorSet> m_descriptorWriteSet1(2);

        //modelUBO
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = model_uniformBuffers[1][frame];
        bufferInfo.offset = 0;
        bufferInfo.range = GetAlignment(sizeof(modelUBO), physicalDevice) * MaximumObjectsOnFrame;

        m_descriptorWriteSet1[0] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, model_descriptorSets[1][frame], &bufferInfo, 0, 0, 1);

        //Texture + Sampler

        VkDescriptorImageInfo imageInfo{};

        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture.textureImageView;
        imageInfo.sampler = sampler;


        m_descriptorWriteSet1[1] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, model_descriptorSets[1][frame], 0, &imageInfo, 1, 1);

        vkUpdateDescriptorSets(device, 2, m_descriptorWriteSet1.data(), 0, nullptr);
    }
}

void UniformBuffer::updateBasicTextureLoaded(VkDevice device, VkPhysicalDevice physicalDevice, VkSampler sampler, std::map<std::string, textureData>& textures, textureData& glitched) {
    //set 1

    for (size_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT; frame++) {
        std::vector< VkWriteDescriptorSet> m_descriptorWriteSet1(4);

        //modelUBO
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[1][frame];
        bufferInfo.offset = 0;
        bufferInfo.range = GetAlignment(sizeof(modelUBO), physicalDevice) * MaximumObjectsOnFrame;

        m_descriptorWriteSet1[0] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, descriptorSets[1][frame], &bufferInfo, 0, 0, 1);

        //Textures
        std::vector<VkDescriptorImageInfo> imageInfo;

        imageInfo.push_back(VkDescriptorImageInfo{});
        imageInfo.back().imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.back().imageView = glitched.textureImageView;
        imageInfo.back().sampler = nullptr;

        uint32_t textureIndex = 1;//index 0 is reserved for invalid texture
        for (auto& texture : textures) {
            imageInfo.push_back(VkDescriptorImageInfo{});
            imageInfo.back().imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.back().imageView = texture.second.textureImageView;
            imageInfo.back().sampler = nullptr;

            texture.second.index = textureIndex;

            textureIndex++;
        }

        imageInfo.resize(MaximumTextures+1);
        for (int i = textures.size()+1; i < MaximumTextures+1; i++) {
            imageInfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo[i].imageView = glitched.textureImageView;
            imageInfo[i].sampler = nullptr;
        }

        m_descriptorWriteSet1[1] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, descriptorSets[1][frame], 0, imageInfo.data(), 1, MaximumTextures+1);

        //sampler
        VkDescriptorImageInfo samplerInfo;
        samplerInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        samplerInfo.imageView = nullptr;
        samplerInfo.sampler = sampler;

        m_descriptorWriteSet1[2] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_SAMPLER, descriptorSets[1][frame], 0, &samplerInfo, 2, 1);

        //textureIndexing
        VkDescriptorBufferInfo bufferInfo2{};
        bufferInfo2.buffer = uniformBuffers[1][frame + 2];
        bufferInfo2.offset = 0;
        bufferInfo2.range = GetAlignment(sizeof(int), physicalDevice) * MaximumObjectsOnFrame;

        m_descriptorWriteSet1[3] = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, descriptorSets[1][frame], &bufferInfo2, 0, 3, 1);

        vkUpdateDescriptorSets(device, 4, m_descriptorWriteSet1.data(), 0, nullptr);
    }
}

void UniformBuffer::updateModelTexture(VkDevice device, VkPhysicalDevice physicalDevice, VkSampler sampler, textureData& texture) {

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