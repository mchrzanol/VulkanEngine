#pragma once

#include "Libraries.h"
#include "VulkanClass/VulkanClass.h"
#include "UniformBuffer.h"
#include "VertexIndexBuffer.h"
#include "window/window.h"
#include "BasicObjects/2D/2DObjects.h"

extern GlobalUtl utils;

class ENGINE_API CommandPool {
private:
    VulkanClass* initVulkan;
    UniformBuffer* initUniform;
    windowClass* initWindow;


    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentFrame = 0;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    int MAX_FRAMES_IN_FLIGHT;
public:
    CommandPool(VulkanClass*& initVulkan, UniformBuffer*& initUniform, windowClass*& initWindow, int MAX_FRAMES_IN_FLIGHT)
        :initVulkan(initVulkan), initUniform(initUniform),initWindow(initWindow),MAX_FRAMES_IN_FLIGHT(MAX_FRAMES_IN_FLIGHT){};
    void createCommandPool();
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass& renderPass,
        std::vector<VkFramebuffer>& swapChainFramebuffers, VkPipeline& graphicsPipeline, VkBuffer vertexBuffer, VkBuffer indexBuffer, std::vector<uint16_t> indices);
    void createSyncObjects();

    void drawObject(VkCommandBuffer commandBuffer, VkBuffer vertexBuffer, VkBuffer indexBuffer, std::vector<uint16_t> indices);

    void drawFrame(VkBuffer vertexBuffer, VkBuffer indexBuffer, std::vector<uint16_t> indices);

    void cleanup();

    VkCommandPool GetCommandPool() { return commandPool; };


};