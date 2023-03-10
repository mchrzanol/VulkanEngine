#pragma once

#include "Libraries.h"
#include "Buffers.h"

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        return attributeDescriptions;
    }
};

class ENGINE_API VertexBuffer {
private:
    const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

public:
    void createVertexBuffer(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice);

    VkBuffer GetVertexBuffer() { return vertexBuffer; };
};

class ENGINE_API IndexBuffer {
private:
    const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
    };
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

public:
    void createIndexBuffer(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice);

    VkBuffer GetIndexBuffer() { return indexBuffer; };
    std::vector<uint16_t> GetIndicies() { return indices; };
};