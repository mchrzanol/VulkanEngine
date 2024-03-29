#pragma once

#include "Libraries.h"
#include "Buffers.h"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
//    glm::mat4 model;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        //attributeDescriptions[2].binding = 0;
        //attributeDescriptions[2].location = 2;
        //attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        //attributeDescriptions[2].offset = offsetof(Vertex, model);
        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

class ENGINE_API VertexBuffer {
private:
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;


public:
    void createVertexBuffer(std::vector<Vertex> & vertices, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool CommandPool);

    void cleanup(VkDevice device);

    VkBuffer GetVertexBuffer() { return vertexBuffer; };
};

class ENGINE_API IndexBuffer {
private:
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<uint32_t> m_indices;

public:

    void createIndexBuffer(std::vector<uint32_t> indices, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool CommandPool);

    void cleanup(VkDevice device);

    VkBuffer GetIndexBuffer() { return indexBuffer; };
    std::vector<uint32_t> GetIndicies() { return m_indices; };
};