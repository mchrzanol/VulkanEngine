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

class ENGINE_API UniformBuffer {
private:
	VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    int MAX_FRAMES_IN_FLIGHT;

public:
    UniformBuffer(int MAX_FRAMES_IN_FLIGHT)
        :MAX_FRAMES_IN_FLIGHT(MAX_FRAMES_IN_FLIGHT) {};
	void createDecriptorSetLayout(VkDevice device);
    void createUnifromBuffer();
    void updateUniformBuffer(uint32_t currentImage, VkExtent2D swapChainExtent);
    void createDescriptorPool();
    void createDescriptorSets();

    void cleanup();

    std::vector<VkDescriptorSet> GetDescriptorSets() { return descriptorSets; };
    VkDescriptorSetLayout& GetDescriptorSetLayout() { return descriptorSetLayout; };
};