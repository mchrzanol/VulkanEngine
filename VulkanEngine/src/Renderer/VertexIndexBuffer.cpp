#include "VertexIndexBuffer.h"

void VertexBuffer::createVertexBuffer() {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, utils.GetDevice(), utils.GetPhyscicalDevice());

    void* data;
    vkMapMemory(utils.GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(utils.GetDevice(), stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory, utils.GetDevice(), utils.GetPhyscicalDevice());

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize, utils.GetCommandPool(), utils.GetDevice(), utils.GetGraphicsQueue());

    vkDestroyBuffer(utils.GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(utils.GetDevice(), stagingBufferMemory, nullptr);
}

void VertexBuffer::cleanup() {
    vkDestroyBuffer(utils.GetDevice(), vertexBuffer, nullptr);
    vkFreeMemory(utils.GetDevice(), vertexBufferMemory, nullptr);
}

void IndexBuffer::createIndexBuffer() {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, utils.GetDevice(), utils.GetPhyscicalDevice());

    void* data;
    vkMapMemory(utils.GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(utils.GetDevice(), stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory, utils.GetDevice(), utils.GetPhyscicalDevice());

    copyBuffer(stagingBuffer, indexBuffer, bufferSize, utils.GetCommandPool(), utils.GetDevice(), utils.GetGraphicsQueue());

    vkDestroyBuffer(utils.GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(utils.GetDevice(), stagingBufferMemory, nullptr);
}

void IndexBuffer::cleanup() {
    vkDestroyBuffer(utils.GetDevice(), indexBuffer, nullptr);
    vkFreeMemory(utils.GetDevice(), indexBufferMemory, nullptr);
}