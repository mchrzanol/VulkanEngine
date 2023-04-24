#pragma once
#include "2D/Triangle.h"
#include "2D/Rectangle.h"
#include "VulkanStructure/VulkanStructure.h"

struct data {
	const static uint32_t MaximumObjectsOnFrame = 20000;

	std::string BasicShaders[2] = {
		"../shaders/BasicVert.spv",
		"../shaders/BasicFrag.spv"
	};

	std::string CircleShaders[2] = {
		"",
		""
	};
};


struct stats
{
	uint32_t EntitiesCount = 0;

	uint32_t triangleCount = 0;
	uint32_t rectangleCount = 0;
};

struct UniformBufferObject {
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct ubo_model {
	glm::mat4* model = nullptr;
};

class ENGINE_API Objects {
private:

	UniformBuffer* initUniform;

	data m_data;
	stats m_stats;

	std::vector<EntityInfo> m_objects;

	ubo_model models;

	size_t alignment;

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	VulkanStruct* VulkanCore;

public:
	Objects(int MAX_FRAMES_IN_FLIGHT) 
	{
		initUniform = new UniformBuffer(MAX_FRAMES_IN_FLIGHT, m_data.MaximumObjectsOnFrame);

		initUniform->createUniformBind(0, sizeof(UniformBufferObject), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, TypeOfUniform::GlobalUniform);
		initUniform->createUniformBind(0, sizeof(glm::mat4), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, TypeOfUniform::EntityUniform);

	}

	void Init(VulkanStruct* Vulkan) {
		VulkanCore = Vulkan;

		m_objects.reserve(m_data.MaximumObjectsOnFrame);
		m_stats.EntitiesCount = 0;

		viewMatrix = glm::mat4(1.f);
		projMatrix = glm::mat4(1.f);

		alignment = initUniform->GetAlignment(sizeof(glm::mat4));
		size_t bufferSize = m_data.MaximumObjectsOnFrame * alignment;
		models.model = (glm::mat4*)_aligned_malloc(bufferSize, alignment);

		VulkanCore->m_Pipeline.createGraphicsPipeline(this->VulkanCore->device, "BasicShader", m_data.BasicShaders[0], m_data.BasicShaders[1], initUniform->descriptorSetLayouts);

	}

	template<typename T>
	void PushBack(T * object) {
		throw std::runtime_error("Undefined Object/Valid Arguments");
	}

	template<>
	void PushBack<EntityInfo>(EntityInfo * object) {
		m_objects.push_back(*object);

		glm::mat4* modelMat = (glm::mat4*)(((uint64_t)models.model + (m_stats.EntitiesCount * alignment)));
		*modelMat = glm::mat4(1.f);

		m_stats.EntitiesCount++;
	}

	void UpdateView(glm::mat4 viewMatrix) {
		this->viewMatrix = viewMatrix;
	}

	void UpdateProjection(glm::mat4 projMatrix) {
		this->projMatrix = projMatrix;
	}

	void rotate(uint32_t EntityIndex, glm::f32 radians, glm::vec3 axis) {
		glm::mat4* modelMat = (glm::mat4*)(((uint64_t)models.model + (EntityIndex * alignment)));

		*modelMat = glm::translate(*modelMat, m_objects[EntityIndex].origin);
		*modelMat = glm::rotate(*modelMat, radians, axis);
		*modelMat = glm::translate(*modelMat, glm::vec3(-1.f) * m_objects[EntityIndex].origin);

	}

	void destroy() {
		_aligned_free(models.model);

		m_objects.clear();
		initUniform->cleanup(VulkanCore->device);
	}

	inline int GetTrianglesSize() { return m_stats.triangleCount; };
	inline int GetRectanglesSize() { return m_stats.rectangleCount; };
	inline int GetEntitiesCount() { return m_stats.EntitiesCount; };

	inline UniformBuffer* GetUniformBuffer() { return initUniform; };

		//cos takiego jak przy registerclass

private:
	void drawObject(VkCommandBuffer& commandBuffer, VkBuffer vertexBuffer, VkBuffer indexBuffer, std::vector<uint16_t> indices) {
		VkBuffer vertexBuffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);//uint16 as type of variable

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	}

	friend class CommandPool;
	void draw2DObjects(VkCommandBuffer& commandBuffer, uint32_t currentFrame) {

		UniformBufferObject ubo = { viewMatrix, projMatrix };
		initUniform->updateStaticUniformBuffer(0, TypeOfUniform::GlobalUniform, ubo, currentFrame);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.pipelineLayout, 0, 1, &initUniform->GetDescriptorSets()[0][currentFrame], 0, nullptr);

		initUniform->updateDynamicUniformBuffer(0, TypeOfUniform::EntityUniform, models.model, currentFrame,m_stats.EntitiesCount);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.graphicsPipeline["BasicShader"]);

		for (size_t index = 0; index < m_stats.EntitiesCount; index++) {
			uint32_t dynamicOffset = index * static_cast<uint32_t>(alignment);

			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.pipelineLayout, 1, 1, &initUniform->GetDescriptorSets()[1][currentFrame], 1, &dynamicOffset);
			drawObject(commandBuffer, m_objects[index].vertices.GetVertexBuffer(), m_objects[index].indices.GetIndexBuffer(),
				m_objects[index].indices.GetIndicies());
		}

	}

};