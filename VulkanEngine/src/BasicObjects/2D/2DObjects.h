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

	std::vector<std::unique_ptr<triangle>> m_triangles;
public:
	~Objects2D() {
		m_triangles.clear();
	}
	//line
	//circle
	//rectanngle

	void PushTriangleBack() {
		m_triangles.push_back(std::make_unique<triangle>());
	}

	void PushTriangleBack(std::unique_ptr<triangle> & object) {
		m_triangles.push_back(std::move(object));
	}

	void draw2DObjects(VkCommandBuffer & commandBuffer, VkPipelineLayout pipelineLayout, VkDescriptorSet& descriptorSet) {
		for (auto it = m_triangles.begin(); it != m_triangles.end(); it++)
			drawObject(commandBuffer, it._Ptr->get()->GetVertexBuffer(), it._Ptr->get()->GetIndexBuffer(),
				it._Ptr->get()->GetIndices(), pipelineLayout, descriptorSet);

	}

	void destroy() {
		m_triangles.clear();
	}

	inline int GetTriangleSize() { return m_triangles.size(); };
		//cos takiego jak przy registerclass

};