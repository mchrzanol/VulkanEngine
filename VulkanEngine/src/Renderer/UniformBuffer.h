#pragma once

#include "Libraries.h"
#include "Buffers.h"

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

    int MAX_FRAMES_IN_FLIGHT;

    uint32_t MaximumObjectsOnFrame;

    std::vector<std::vector< VkDescriptorSetLayoutBinding>> uboLayoutBinding;
    std::vector < std::vector<VkBuffer>> uniformBuffers;

    std::vector < std::vector<VkDescriptorPoolSize>> poolSize;
    std::vector < VkDescriptorPool> descriptorPool;

    std::vector < std::vector<VkDescriptorSet>> descriptorSets;

    std::vector<UniformList> StaticUniformList;
    std::vector<UniformList> DynamicUniformList;

    std::vector < std::map<unsigned int, BindingData>> BindingData;//binding, bindingdata

    std::vector<std::vector<unsigned int>> bindingQueue;

    std::vector<unsigned int> UniformsCount = { 0,0 };


public:
    std::vector < std::vector<void*>> uniformBuffersMapped;
    std::vector < std::vector<VkDeviceMemory>> uniformBuffersMemory;
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

	void createDecriptorSetsLayout(VkDevice device);

    void createUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice);
    void createDescriptorPool(VkDevice device);
    void createDescriptorSets(VkDevice device, VkPhysicalDevice physicalDevice);

    size_t GetAlignment(size_t sizeOfData, VkPhysicalDevice physicalDevice) {
        VkPhysicalDeviceProperties deviceProps;

        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProps);
        size_t minUboAlignment = deviceProps.limits.minUniformBufferOffsetAlignment;

        size_t dynamicAlignment = sizeOfData;
        if (minUboAlignment > 0) {
            dynamicAlignment = (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
        }
        return dynamicAlignment;
    };

    template<class T>
    void updateStaticUniformBuffer(unsigned int binding, TypeOfUniform UniformType, T& data, uint32_t currentImage) {

        unsigned int MappedOffset = UniformsCount[static_cast<int>(UniformType)];

        memcpy(uniformBuffersMapped[static_cast<int>(UniformType)][binding + (currentImage * MappedOffset)], &data, BindingData[static_cast<int>(UniformType)][binding].sizeofData);
    }

    template<class T>
    void updateArrayUniformBuffer(unsigned int binding, TypeOfUniform UniformType, T& data, uint32_t currentImage, uint32_t countOfData, size_t alignment) {

        unsigned int MappedOffset = binding + (currentImage * UniformsCount[static_cast<int>(UniformType)]);

        void* buffer = uniformBuffersMapped[static_cast<int>(UniformType)][MappedOffset];

        memcpy(buffer, &data, alignment * countOfData);
    };


    template<class T>
    void updateDynamicUniformBuffer(VkDevice device, VkPhysicalDevice physicalDevice, unsigned int binding, TypeOfUniform UniformType, T& data, uint32_t currentImage, uint32_t countOfData , uint32_t offset = 0) {
        size_t alignment = GetAlignment(BindingData[static_cast<int>(UniformType)][binding].sizeofData, physicalDevice);

        VkDeviceSize calculateOffset = offset * alignment;

        unsigned int MappedOffset = UniformsCount[static_cast<int>(UniformType)];

        void* buffer = uniformBuffersMapped[static_cast<int>(UniformType)][binding + (currentImage * MappedOffset)];

        memcpy(buffer, data + offset, countOfData*alignment);

        VkMappedMemoryRange memoryRange;
        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.pNext = 0;
        memoryRange.offset = calculateOffset;
        memoryRange.memory = uniformBuffersMemory[static_cast<int>(UniformType)][(binding + currentImage) * currentImage];
        memoryRange.size = alignment * countOfData;
        vkFlushMappedMemoryRanges(device, 1, &memoryRange);
    };


    void cleanup(VkDevice device);

    std::vector<std::vector<VkDescriptorSet>> GetDescriptorSets() { return descriptorSets; };
    std::vector<VkDescriptorSetLayout> GetDescriptorSetLayouts() { return descriptorSetLayouts; };
};