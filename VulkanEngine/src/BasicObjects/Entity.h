#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"

enum class EntityType {
	Triangle, Rectangle, Circle, Line
};

enum class Orientation {
	X, Y, Z
};

struct EntityData {
	EntityType type;
};

struct EntityVitalInfo {

	uint32_t indexCount;

	glm::vec3 origin;

	glm::mat4 model = glm::mat4(1.f);

	EntityData data;
};

struct EntityInfo {
	VertexBuffer vertices;
	IndexBuffer indices;

	EntityVitalInfo data;

	void createVertexBuffer(std::vector<Vertex>& vertices, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool CommandPool) {
		this->vertices.createVertexBuffer(vertices, device, physicalDevice, graphicsQueue, CommandPool);
	}

	void createIndexBuffer(std::vector<uint16_t> indices, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool CommandPool) {
		this->indices.createIndexBuffer(indices, device, physicalDevice, graphicsQueue,CommandPool);
	}


	//texid
};