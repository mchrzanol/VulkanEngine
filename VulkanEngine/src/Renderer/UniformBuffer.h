#pragma once

#include "Libraries.h"
#include "Buffers.h"
#include "GlobalVariables.h"

extern GlobalUtl utils;

struct UniformBufferObject {
    alignas(16) glm::mat4 model;//alignas(16) = przesuniecie bajtowe(co 16 bajtów)
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

enum class TypeOfUniform {
    GlobalUniform = 0,
    LocalUniform = 1
};

struct BindingData {
    size_t sizeofData;
    VkDescriptorType DescriptorType;
};

class ENGINE_API UniformBuffer {
private:
	//std::vector<VkDescriptorSetLayout> descriptorSetLayouts;

    int MAX_FRAMES_IN_FLIGHT;

    std::vector<std::vector< VkDescriptorSetLayoutBinding>> uboLayoutBinding;
    std::vector < std::vector<VkBuffer>> uniformBuffers;
    std::vector < std::vector<VkDeviceMemory>> uniformBuffersMemory;
    std::vector < std::vector<void*>> uniformBuffersMapped;

    std::vector < std::vector<VkDescriptorPoolSize>> poolSize;
    std::vector < VkDescriptorPool> descriptorPool;

    std::vector < std::vector<VkDescriptorSet>> descriptorSets;

    std::vector < std::map<unsigned int, BindingData>> BindingData;//binding, bindingdata

    std::vector<std::vector<unsigned int>> bindingQueue;

    std::vector<unsigned int> UniformsCount = { 0,0 };


public:
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;

    UniformBuffer(int MAX_FRAMES_IN_FLIGHT)
        :MAX_FRAMES_IN_FLIGHT(MAX_FRAMES_IN_FLIGHT) {
        uboLayoutBinding.resize(2);
        descriptorSetLayouts.resize(2);
        uniformBuffers.resize(2);
        uniformBuffersMemory.resize(2);
        uniformBuffersMapped.resize(2);
        poolSize.resize(2);
        descriptorPool.resize(2);
        descriptorSets.resize(2);
        BindingData.resize(2);
        bindingQueue.resize(2);
    };

    void createUniform(unsigned int binding, size_t sizeofBindingValue, VkDescriptorType DescriptorType, TypeOfUniform UniformType);

	void createDecriptorSetsLayout();

    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();

    template<class T>
    void updateUniformBuffer(unsigned int binding, TypeOfUniform UniformType, T& data, uint32_t currentImage) {

        // memcpy(uniformBuffersMapped[0][currentImage], &data, sizeofObject);// BindingData[0][0].sizeofData);

        memcpy(uniformBuffersMapped[static_cast<int>(UniformType)][binding * (currentImage + 1)], &data, BindingData[static_cast<int>(UniformType)][binding].sizeofData);
    }

    void cleanup();

    std::vector<VkDescriptorSet> GetDescriptorSets() { return descriptorSets[0]; };
    std::vector<VkDescriptorSetLayout> GetDescriptorSetLayouts() { return descriptorSetLayouts; };
};