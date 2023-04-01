#pragma once

#include "Libraries.h"
#include "Buffers.h"
#include "GlobalVariables.h"

extern GlobalUtl utils;

enum class TypeOfUniform {
    GlobalUniform = 0,
    EntityUniform = 1
};

struct BindingData {
    size_t sizeofData;
    VkDescriptorType DescriptorType;
};

struct UniformList {
    unsigned int set;
    unsigned int bind;
};

class ENGINE_API UniformBuffer {
private:
	//std::vector<VkDescriptorSetLayout> descriptorSetLayouts;

    int MAX_FRAMES_IN_FLIGHT;

    uint32_t MaximumObjectsOnFrame;

    std::vector<std::vector< VkDescriptorSetLayoutBinding>> uboLayoutBinding;
    std::vector < std::vector<VkBuffer>> uniformBuffers;
    std::vector < std::vector<VkDeviceMemory>> uniformBuffersMemory;
    std::vector < std::vector<void*>> uniformBuffersMapped;

    std::vector < std::vector<VkDescriptorPoolSize>> poolSize;
    std::vector < VkDescriptorPool> descriptorPool;

    std::vector < std::vector<VkDescriptorSet>> descriptorSets;

    std::vector<UniformList> StaticUniformList;
    std::vector<UniformList> DynamicUniformList;

    std::vector < std::map<unsigned int, BindingData>> BindingData;//binding, bindingdata

    std::vector<std::vector<unsigned int>> bindingQueue;

    std::vector<unsigned int> UniformsCount = { 0,0 };


public:
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;

    UniformBuffer(int MAX_FRAMES_IN_FLIGHT, uint32_t MaximumObjectsOnFrame)
        :MAX_FRAMES_IN_FLIGHT(MAX_FRAMES_IN_FLIGHT), MaximumObjectsOnFrame(MaximumObjectsOnFrame){
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

    void createUniformBind(unsigned int binding, size_t sizeofBindingValue, VkDescriptorType DescriptorType, TypeOfUniform UniformType);

	void createDecriptorSetsLayout();

    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();

    size_t GetAlignment(size_t sizeOfData) {
        size_t minUboAlignment = utils.GetPhysicalDeviceProps().limits.minUniformBufferOffsetAlignment;
        std::cout << utils.GetPhysicalDeviceProps().limits.minUniformBufferOffsetAlignment << std::endl;
        size_t dynamicAlignment = sizeOfData;
        if (minUboAlignment > 0) {
            dynamicAlignment = (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
        }
        return dynamicAlignment;
    };

    template<class T>
    void updateStaticUniformBuffer(unsigned int binding, TypeOfUniform UniformType, T& data, uint32_t currentImage) {

        // memcpy(uniformBuffersMapped[0][currentImage], &data, sizeofObject);// BindingData[0][0].sizeofData);

        memcpy(uniformBuffersMapped[static_cast<int>(UniformType)][binding * (currentImage + 1)], &data, BindingData[static_cast<int>(UniformType)][binding].sizeofData);
    };
    template<class T>
    void updateDynamicUniformBuffer(unsigned int binding, TypeOfUniform UniformType, T& data, uint32_t currentImage, uint32_t countOfData) {

        // memcpy(uniformBuffersMapped[0][currentImage], &data, sizeofObject);// BindingData[0][0].sizeofData);

        memcpy(uniformBuffersMapped[static_cast<int>(UniformType)][binding * (currentImage + 1)], &data, GetAlignment(BindingData[static_cast<int>(UniformType)][binding].sizeofData)*countOfData);

        //VkMappedMemoryRange memoryRange{};
        //memoryRange.memory = uniformBuffersMemory[static_cast<int>(UniformType)][binding * (currentImage + 1)];
        //memoryRange.size = sizeof(uboDataDynamic);
        //vkFlushMappedMemoryRanges(device, 1, &memoryRange);
    };


    void cleanup();

    std::vector<std::vector<VkDescriptorSet>> GetDescriptorSets() { return descriptorSets; };
    std::vector<VkDescriptorSetLayout> GetDescriptorSetLayouts() { return descriptorSetLayouts; };
};