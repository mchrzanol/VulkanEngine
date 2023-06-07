#include "VulkanStructure.h"

void VulkanStruct::checkExtensions() {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "available extensions:\n";

    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }
}

std::vector<const char*> VulkanStruct::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void VulkanStruct::createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    checkExtensions();

    //App Info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;
    appInfo.pNext = nullptr;


    //Create Info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }


}

//ValidationLayer
void VulkanStruct::setupDebugMessenger() {
    if (!enableValidationLayers) 
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

//surface
void VulkanStruct::createSurface() {
    if (glfwCreateWindowSurface(instance, window->m_window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

bool VulkanStruct::checkValidationLayerSupport() {
    //Checks if all requested validation layers are available

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }
    return true;
}

void VulkanStruct::recreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window->m_window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window->m_window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);

    m_DepthBuffer.cleanup(device);
    m_FrameBuffer.cleanup(device);
    m_SwapChain.cleanupSwapChain(device);

    m_SwapChain.createSwapChain(m_Hardwaredevice.physicalDevice, surface, device, window->m_window,
        m_Hardwaredevice.findQueueFamilies(m_Hardwaredevice.physicalDevice, surface));
    m_SwapChain.createImageViews(device);
    m_DepthBuffer.createDepthBuffer(device, m_Hardwaredevice.physicalDevice, m_SwapChain.swapChainExtent);
    m_FrameBuffer.createFramebuffer(device, m_SwapChain.swapChainImageViews, m_DepthBuffer.depthImageView, m_SwapChain.swapChainExtent, m_Pipeline.renderPass);
}
