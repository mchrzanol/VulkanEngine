#pragma once

#include "logging/logging.h"
#include "window/window.h"
#include "Libraries.h"
#include "Renderer/UniformBuffer.h"

#include "PhysicalLogicalDevice/PhysicalLogicalDevice.h"
#include "SwapChain/SwapChain.h"
#include "DepthBuffer/DepthBuffer.h"
#include "FrameBuffer/FrameBuffer.h"
#include "Pipeline/Pipeline.h"


class ENGINE_API VulkanStruct {
private:
    const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
    };

    friend class CommandPool;
    friend class Objects;
public:
	VulkanStruct(windowClass*& window)
		:window(window) {}

    void Init(UniformBuffer* uniformBuffer) {
        createInstance();
        setupDebugMessenger();
        createSurface();

        m_Hardwaredevice.pickPhysicalDevice(instance, surface);
        m_Hardwaredevice.createLogicalDevice(validationLayers, surface, device);

        m_SwapChain.createSwapChain(m_Hardwaredevice.physicalDevice, surface, device, window->m_window,
            m_Hardwaredevice.findQueueFamilies(m_Hardwaredevice.physicalDevice, surface));

        m_SwapChain.createImageViews(device);

        m_Pipeline.createRenderPass(device, m_DepthBuffer.findDepthFormat(m_Hardwaredevice.physicalDevice), m_SwapChain.swapChainImageFormat);

        uniformBuffer->createDecriptorSetsLayout(device);

        m_DepthBuffer.createDepthBuffer(device, m_Hardwaredevice.physicalDevice, m_SwapChain.swapChainExtent);
        m_FrameBuffer.createFramebuffer(device, m_SwapChain.swapChainImageViews, m_DepthBuffer.depthImageView, m_SwapChain.swapChainExtent, m_Pipeline.renderPass);

        //uniformBuffer->createUniformBuffers(device, m_Hardwaredevice.physicalDevice);
        //uniformBuffer->createDescriptorPool(device);
       // uniformBuffer->createDescriptorSets(device, m_Hardwaredevice.physicalDevice);
    }

    void cleanup() {
        m_DepthBuffer.cleanup(device);
        m_FrameBuffer.cleanup(device);
        m_SwapChain.cleanupSwapChain(device);

        m_Pipeline.cleanup(device);

        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        vkDestroyDevice(device, nullptr);

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }

    inline VkExtent3D GetSwapChainExtent() { return m_SwapChain.swapChainExtent; };

    inline void WaitIdle() { vkDeviceWaitIdle(device); };

private:
    windowClass* window;
	VkDevice device;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

#pragma region Members
    PickingDevice m_Hardwaredevice;
    SwapChain m_SwapChain;
    depthBuffer m_DepthBuffer;
    frameBuffer m_FrameBuffer;
    pipeline m_Pipeline;

#pragma endregion

private:
    //Instance
    void checkExtensions();
    std::vector<const char*> getRequiredExtensions();
    void createInstance();

    //ValidationLayer
    void setupDebugMessenger();

    //Surface
    void createSurface();

    //Debugger
    bool checkValidationLayerSupport();

    //SwapChain
    void recreateSwapChain();
};