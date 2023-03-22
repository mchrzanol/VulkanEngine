#pragma once
#include "Triangle.h"
#include "Rectangle.h"

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
	std::vector<std::unique_ptr<rectangle>> m_rectangles;
	//line
	//circle
public:
	template<typename T>
	void PushBack() {
		throw std::runtime_error("Undefined 2D Object!");//maybe some warning instead of this
	}

	template<>
	void PushBack<triangle>() {
			m_triangles.push_back(std::make_unique<triangle>());
	}

	template<>
	void PushBack<rectangle>() {
		m_rectangles.push_back(std::make_unique<rectangle>());
	}


	template<typename T>
	void PushBack(std::unique_ptr<T>& object) {
		throw std::runtime_error("Undefined 2D Object!");
	}

	template<>
	void PushBack<triangle>(std::unique_ptr<triangle>& object) {
		m_triangles.push_back(std::move(object));
	}

	template<>
	void PushBack<rectangle>(std::unique_ptr<rectangle>& object) {
		m_rectangles.push_back(std::move(object));
	}

	void draw2DObjects(VkCommandBuffer & commandBuffer, VkPipelineLayout pipelineLayout, VkDescriptorSet& descriptorSet) {
		for (auto it = m_triangles.begin(); it != m_triangles.end(); it++)//triangles
			drawObject(commandBuffer, it._Ptr->get()->GetVertexBuffer(), it._Ptr->get()->GetIndexBuffer(),
				it._Ptr->get()->GetIndices(), pipelineLayout, descriptorSet);

		for (auto it = m_rectangles.begin(); it != m_rectangles.end(); it++)//rectangles
			drawObject(commandBuffer, it._Ptr->get()->GetVertexBuffer(), it._Ptr->get()->GetIndexBuffer(),
				it._Ptr->get()->GetIndices(), pipelineLayout, descriptorSet);

	}

	void destroy() {
		m_triangles.clear();
		m_rectangles.clear();
	}

	inline int GetTrianglesSize() { return m_triangles.size(); };
	inline int GetRectanglesSize() { return m_rectangles.size(); };

	inline std::unique_ptr<triangle>& GetTriangle(uint16_t index) { return m_triangles[index]; }
	inline std::unique_ptr<rectangle>& GetRectangle(uint16_t index) { return m_rectangles[index]; }
		//cos takiego jak przy registerclass

};