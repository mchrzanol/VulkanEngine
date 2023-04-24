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
        utils.BindSwapChainImageFormat(swapChainImageFormat);
        createImageViews();

        utils.BindDevice(device);
        utils.BindPhysicalDevice(physicalDevice);
        uniformBuffer->createDecriptorSetsLayout();
        createDepthBuffer();
        createFramebuffers();

        utils.BindGraphicsQueue(graphicsQueue);

        uniformBuffer->createUniformBuffers();
        uniformBuffer->createDescriptorPool();
        uniformBuffer->createDescriptorSets();
	}

    void cleanup() {

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
    VkExtent3D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
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
    VkExtent3D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createSwapChain();
    void createImageViews();

    //FrameBuffer
    void createFramebuffers();

    //DepthBuffer
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    friend class pipeline;
    static VkFormat findDepthFormat();
    bool hasStencilComponent(VkFormat format);
    void createDepthBuffer();
    void createImage(VkExtent3D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    bool checkValidationLayerSupport();

public:
    //Physical / Logical Device
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void recreateSwapChain();
    void recreateDepthBuffer();

    inline VkPhysicalDevice GetPhyscicalDevice() { return physicalDevice; };
    inline VkDevice& GetDevice() { return device; };
    inline VkExtent3D GetSwapChainExtent() { return swapChainExtent; };
    inline VkQueue GetGraphicsQueue() { return graphicsQueue; };
    inline VkSwapchainKHR GetSwapChain() { return swapChain;};
    inline std::vector<VkFramebuffer> GetSwapChainFrameBuffers() {return swapChainFramebuffers; };
    inline VkQueue GetPresentQueue() { return presentQueue; };

    inline void WaitIdle() { vkDeviceWaitIdle(device); };

};