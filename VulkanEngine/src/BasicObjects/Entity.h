#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"

enum class EntityType {
	Triangle = 3, Rectangle = 4, Circle = 1, Line = 2//by vertexcount
};

enum class Orientation {
	X, Y, Z
};

struct EntityVitalInfo {
	glm::vec3 origin;

	glm::mat4 model = glm::mat4(1.f);

	glm::vec3 color;

	std::string textureName = "";

	uint16_t vertexCount;

	std::string ID;
};

struct EntityInfo {
	VertexBuffer vertices;
	IndexBuffer indices;

	EntityVitalInfo data;

	void createVertexBuffer(std::vector<Vertex>& vertices, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool CommandPool) {
		this->vertices.createVertexBuffer(vertices, device, physicalDevice, graphicsQueue, CommandPool);
	}

	void createIndexBuffer(std::vector<uint32_t> indices, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool CommandPool) {
		this->indices.createIndexBuffer(indices, device, physicalDevice, graphicsQueue,CommandPool);
	}


	//texid
};