#pragma once

#include "logging/logging.h"
#include "window/window.h"
#include "Libraries.h"
#include "Renderer/UniformBuffer.h"

extern GlobalUtl utils;

#pragma warning(disable: 26495)

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


class ENGINE_API VulkanClass {
public:
    VulkanClass(GLFWwindow*& window, UniformBuffer* uniformBuffer)//including reference to window
        :window(window), uniformBuffer(uniformBuffer) {};
	void init() {
        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createSwapChain();
        createImageViews();

        createRenderPass();
        utils.BindDevice(device);
        utils.BindPhysicalDevice(physicalDevice);
        uniformBuffer->createDecriptorSetsLayout();
        createGraphicsPipeline();
        createFramebuffers();

        utils.BindGraphicsQueue(graphicsQueue);

        uniformBuffer->createUniformBuffers();
        uniformBuffer->createDescriptorPool();
        uniformBuffer->createDescriptorSets();
	}

    void cleanup() {

        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyRenderPass(device, renderPass, nullptr);


        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        vkDestroyDevice(device, nullptr);

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }

    void cleanupSwapChain();

private:
    #pragma region externalMembers
    GLFWwindow* window;
    UniformBuffer* uniformBuffer;
    #pragma endregion

    #pragma region Members
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkPipeline graphicsPipeline;
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;

    uint32_t currentFrame = 0;
    #pragma endregion

    //Instance
    void checkExtensions();
    std::vector<const char*> getRequiredExtensions();
    void createInstance();

    //ValidationLayer
    void setupDebugMessenger();

    //Surface
    void createSurface();
    
    //Physical / Logical Device

    void pickPhysicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice device);
    void createLogicalDevice();
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    //SwapChain
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createSwapChain();
    void createImageViews();

    //Pipeline
    void createRenderPass();
    void createGraphicsPipeline();

    //FrameBuffer
    void createFramebuffers();
    
    bool checkValidationLayerSupport();

public:
    //Physical / Logical Device
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void recreateSwapChain(WindowData data);

    inline VkPhysicalDevice GetPhyscicalDevice() { return physicalDevice; };
    inline VkDevice& GetDevice() { return device; };
    inline VkExtent2D GetSwapChainExtent() { return swapChainExtent; };
    inline VkPipelineLayout GetPipelineLayout() { return pipelineLayout; };
    inline VkQueue GetGraphicsQueue() { return graphicsQueue; };
    inline VkPipeline& GetGraphicsPipeline() { return graphicsPipeline; };
    inline VkSwapchainKHR GetSwapChain() { return swapChain;};
    inline VkRenderPass& GetRenderPass() { return renderPass; };
    inline std::vector<VkFramebuffer> GetSwapChainFrameBuffers() {return swapChainFramebuffers; };
    inline VkQueue GetPresentQueue() { return presentQueue; };

    inline void WaitIdle() { vkDeviceWaitIdle(device); };

};