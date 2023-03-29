#pragma once
#include "2D/Triangle.h"
#include "2D/Rectangle.h"

struct stats
{
	uint32_t MaximumObjectsOnFrame = 20000;
	uint32_t EntitiesCount = 0;

	uint32_t triangleCount = 0;
	uint32_t rectangleCount = 0;
};

enum class EntityType {
	Triangle, Rectangle, Circle, Line
};

struct Entity {
	VertexBuffer vertices;
	IndexBuffer indices;

	glm::mat4 model = glm::mat4(1.f);

	void createVertexBuffer(std::vector<Vertex>& vertices) {
		this->vertices.createVertexBuffer(vertices);
	}

	void createIndexBuffer(std::vector<uint16_t>& indices) {
		this->indices.createIndexBuffer(indices);
	}

	//texid
};

struct EntityData {
	EntityType type;
};

class Objects {
private:
	void drawObject(VkCommandBuffer & commandBuffer, VkBuffer vertexBuffer, VkBuffer indexBuffer, std::vector<uint16_t> indices,
		VkPipelineLayout pipelineLayout) {
		VkBuffer vertexBuffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);//uint16 as type of variable

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	}

	//Data m_data;
	stats m_stats;

	std::vector<Entity> m_objects;
	std::map<uint32_t, EntityData> m_data;

	//line
	//circle
public:
	Objects() {
		m_objects.reserve(m_stats.MaximumObjectsOnFrame);
		m_stats.EntitiesCount = 0;
	}

	template<typename T>
	void PushBack(T * object) {
		throw std::runtime_error("Undefined 2D Object!");
	}

	template<>
	void PushBack<triangle>(triangle * object) {

		m_objects.push_back(Entity());
		m_objects[m_stats.EntitiesCount].createVertexBuffer(object->GetVertices());
		m_objects[m_stats.EntitiesCount].createIndexBuffer(object->GetIndices());

		m_data[m_stats.EntitiesCount] = { EntityType::Triangle };

		m_stats.EntitiesCount++;
		m_stats.triangleCount++;
	}

	template<>
	void PushBack<rectangle>(rectangle * object) {
		m_objects.push_back(Entity());
		m_objects[m_stats.EntitiesCount].createVertexBuffer(object->GetVertices());
		m_objects[m_stats.EntitiesCount].createIndexBuffer(object->GetIndices());

		m_data[m_stats.EntitiesCount] = { EntityType::Rectangle };

		m_stats.EntitiesCount++;
		m_stats.rectangleCount++;
	}

	void draw2DObjects(VkCommandBuffer & commandBuffer, VkPipelineLayout pipelineLayout, VkDescriptorSet& descriptorSet) {

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
		for (size_t index = 0; index < m_stats.EntitiesCount; index++) {
			drawObject(commandBuffer, m_objects[index].vertices.GetVertexBuffer(), m_objects[index].indices.GetIndexBuffer(),
				m_objects[index].indices.GetIndicies(), pipelineLayout);
		}

		//std::cout << m_objects.size() << std::endl;

	}

	void destroy() {
		m_objects.clear();
	}

	inline int GetTrianglesSize() { return m_stats.triangleCount; };
	inline int GetRectanglesSize() { return m_stats.rectangleCount; };

		//cos takiego jak przy registerclass

};