#pragma once

#include "Libraries.h"
#include "VulkanClass/VulkanClass.h"
#include "UniformBuffer.h"
#include "VertexIndexBuffer.h"
#include "window/window.h"

class ENGINE_API CommandPool {
private:
    VulkanClass* initVulkan;
    UniformBuffer* initUniform;
    IndexBuffer* initIndeces;
    VertexBuffer* initVertices;
    windowClass* initWindow;


    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentFrame = 0;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    int MAX_FRAMES_IN_FLIGHT;
public:
    CommandPool(VulkanClass*& initVulkan, UniformBuffer*& initUniform, IndexBuffer*& initIndeces, VertexBuffer*& initVertices, windowClass*& initWindow, int MAX_FRAMES_IN_FLIGHT)
        :initVulkan(initVulkan), initUniform(initUniform), initIndeces(initIndeces), initVertices(initVertices),initWindow(initWindow),MAX_FRAMES_IN_FLIGHT(MAX_FRAMES_IN_FLIGHT){};
    void createCommandPool();
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass& renderPass,
        std::vector<VkFramebuffer>& swapChainFramebuffers, VkPipeline& graphicsPipeline);
    void createSyncObjects();

    void drawFrame();

    void cleanup();

    VkCommandPool GetCommandPool() { return commandPool; };


};