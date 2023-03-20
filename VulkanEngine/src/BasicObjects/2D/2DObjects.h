#pragma once
#include "Triangle.h"

//struct stats
//{
//	uint32_t valueOfObject;
//};
//
//struct Data
//{
//	uint32_t QuadIndexCount = 0;
//
//	//jakies tekstury czy chuj wie co
//};

class Objects2D {
private:
	//Data m_data;
	//stats m_stats;
	void drawObject(VkCommandBuffer & commandBuffer, VkBuffer vertexBuffer, VkBuffer indexBuffer, std::vector<uint16_t> indices,
		VkPipelineLayout pipelineLayout, VkDescriptorSet & descriptorSet) {
		VkBuffer vertexBuffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);//uint16 as type of variable

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	}

public:
	std::vector<triangle> m_triangles;

	void PushTriangleBack() {
		m_triangles.push_back(triangle());
	}

	void draw2DObjects(VkCommandBuffer & commandBuffer, VkPipelineLayout pipelineLayout, VkDescriptorSet& descriptorSet) {
		for (auto triangle : m_triangles)
			drawObject(commandBuffer, triangle.GetVertexBuffer(), triangle.GetIndexBuffer(),
				triangle.GetIndices(), pipelineLayout, descriptorSet);

	}
		//cos takiego jak przy registerclass

};