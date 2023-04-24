#pragma once

#include "Libraries.h"
#include "VulkanStructure/VulkanStructure.h"
#include "UniformBuffer.h"
#include "VertexIndexBuffer.h"
#include "window/window.h"
#include "BasicObjects/Objects.h"

extern GlobalUtl utils;

class ENGINE_API CommandPool {
private:
    VulkanStruct* VulkanCore;
    windowClass* Window;


    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentFrame = 0;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    int MAX_FRAMES_IN_FLIGHT;
public:
    CommandPool(VulkanStruct*& Vulkan, windowClass*& Window, int MAX_FRAMES_IN_FLIGHT)
        :VulkanCore(Vulkan),Window(Window),MAX_FRAMES_IN_FLIGHT(MAX_FRAMES_IN_FLIGHT){};
    void createCommandPool();
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex,
        std::vector<VkFramebuffer>& swapChainFramebuffers, Objects& objects);
    void createSyncObjects();

    void drawFrame(Objects& objects);

    void cleanup();

    inline VkCommandPool GetCommandPool() { return commandPool; };

    inline uint32_t GetCurrentFrame() { return currentFrame; };


};