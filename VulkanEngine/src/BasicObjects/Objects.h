#pragma once
#include "2D/Triangle.h"
#include "2D/Rectangle.h"
#include "VulkanStructure/VulkanStructure.h"
const static struct data {
	const static uint32_t MaximumObjectsOnFrame = 10000;

	const std::string BasicShaders[2] = {
		"../shaders/BasicVert.spv",
		"../shaders/BasicFrag.spv"
	};

	const std::string CircleShaders[2] = {
		"",
		""
	};
}m_data;

struct stats
{
	uint32_t EntitiesCount = 0;
	uint32_t VerticesCount = 0;
	uint32_t IndicesCount = 0;

	uint32_t triangleCount = 0;
	uint32_t rectangleCount = 0;
};

struct UniformBufferObject {
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct modelUBO {
	alignas(16) glm::mat4 model;
	alignas(16) glm::vec3 color;
};

class ENGINE_API Objects {
private:

	UniformBuffer* initUniform;

	stats m_stats;

	std::vector<EntityVitalInfo> m_objects;

	bool DynamicUniformsUpdate = true;

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	VulkanStruct* VulkanCore;

	VkCommandPool* CommandPool;

	//drawIndirect

	struct IndirectBatch {
		std::string ID;

		uint32_t vertexCount;

		uint32_t first;//count of previous batch
		uint32_t count;

		std::vector<glm::mat4> model;
		std::vector<glm::vec3> color;
	};

	modelUBO EntityUBO[m_data.MaximumObjectsOnFrame];
	bool EntityUBOchanged = true;

	std::vector<IndirectBatch> batchDraw;
	std::map<std::string, int> indexOfDraw;

	std::map< std::string, VertexBuffer> EntitiesVertexBuffer;

	VkBuffer DrawCommandBuffer;
	VkDeviceMemory DrawCommandBufferMemory;
	void* DrawCommandData;

	VkDrawIndirectCommand* drawCommands = nullptr;

public:
	Objects(int MAX_FRAMES_IN_FLIGHT) 
	{
		initUniform = new UniformBuffer(MAX_FRAMES_IN_FLIGHT, m_data.MaximumObjectsOnFrame);

		initUniform->createUniformBind(0, sizeof(UniformBufferObject), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, TypeOfUniform::GlobalUniform);

		size_t buffersize = sizeof(modelUBO) * m_data.MaximumObjectsOnFrame;

		initUniform->createUniformBind(0, buffersize, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, TypeOfUniform::EntityUniform);

		//size_t buffersize2 = sizeof(glm::vec3) * m_data.MaximumObjectsOnFrame;

		//initUniform->createUniformBind(5, buffersize2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, TypeOfUniform::EntityUniform);

	}

	void Init(VulkanStruct* Vulkan, VkCommandPool* CommandPool) {
		VulkanCore = Vulkan;

		this->CommandPool = CommandPool;

		m_objects.reserve(m_data.MaximumObjectsOnFrame);
		m_stats.EntitiesCount = 0;

		viewMatrix = glm::mat4(1.f);
		projMatrix = glm::mat4(1.f);

		VulkanCore->m_Pipeline.createGraphicsPipeline(this->VulkanCore->device, "BasicShader", m_data.BasicShaders[0], m_data.BasicShaders[1], initUniform->descriptorSetLayouts);

		createBuffers();
	}

	template<typename T>
	void PushBack(T * object) {
		throw std::runtime_error("Undefined Object/Valid Arguments");
	}

	template<>
	void PushBack<EntityVitalInfo>(EntityVitalInfo * object) {

		m_objects.push_back(*object);

		compact_draw(m_stats.EntitiesCount);

		m_stats.EntitiesCount++;


		delete[] drawCommands;
		drawCommands = updateDrawCommand();

		EntityUBOchanged = true;
	}

	void UpdateView(glm::mat4 viewMatrix) {
		this->viewMatrix = viewMatrix;
	}

	void UpdateProjection(glm::mat4 projMatrix) {
		this->projMatrix = projMatrix;
	}

	void rotate() {
		for (int i = 0; i < m_stats.EntitiesCount; i++) {
			EntityUBO[i].model = glm::translate(EntityUBO[i].model, m_objects[i].origin);
			EntityUBO[i].model = glm::rotate(EntityUBO[i].model, glm::radians(10.f), glm::vec3(1, 0, 0));
			EntityUBO[i].model = glm::translate(EntityUBO[i].model, glm::vec3(-1.f) * m_objects[i].origin);
		}

		//*modelMat = glm::translate(*modelMat, m_objects[EntityIndex].data.origin);
		//*modelMat = glm::rotate(*modelMat, radians, axis);
		//*modelMat = glm::translate(*modelMat, glm::vec3(-1.f) * m_objects[EntityIndex].data.origin);
		EntityUBOchanged = true;
	}

	void destroy() {

		for (auto& VertexBuffer : EntitiesVertexBuffer) {
			VertexBuffer.second.cleanup(VulkanCore->device);
		}

		m_objects.clear();
		initUniform->cleanup(VulkanCore->device);

		vkDestroyBuffer(VulkanCore->device, DrawCommandBuffer, nullptr);
		vkFreeMemory(VulkanCore->device, DrawCommandBufferMemory, nullptr);
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

	void createBuffers();
	void addToBuffer(std::vector<Vertex> verticesInfo, std::vector<uint16_t> indices, VkCommandPool CommandPool);

	void createIndirectBuffer();
	void compact_draw(int index);
	VkDrawIndirectCommand* updateDrawCommand();
	void updateUniforms(VkCommandBuffer& commandBuffer, uint32_t currentFrame);
	void drawIndirect(VkCommandBuffer& commandBuffer, uint32_t currentFrame);


	friend class CommandPool;
	void draw2DObjects(VkCommandBuffer& commandBuffer, uint32_t currentFrame) {

		drawIndirect(commandBuffer, currentFrame);

		//UniformBufferObject ubo = { viewMatrix, projMatrix };
		//initUniform->updateStaticUniformBuffer(0, TypeOfUniform::GlobalUniform, ubo, currentFrame);

		//vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.pipelineLayout, 0, 1, &initUniform->GetDescriptorSets()[0][currentFrame], 0, nullptr);

		//if (DynamicUniformsUpdate == true) {
		//	initUniform->updateDynamicUniformBuffer(VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, 0, TypeOfUniform::EntityUniform, models.model, currentFrame, m_stats.EntitiesCount);
		//	DynamicUniformsUpdate = false;
		//}
		//vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.graphicsPipeline["BasicShader"]);

		//for (size_t index = 0; index < m_stats.EntitiesCount; index++) {
		//	uint32_t dynamicOffset = index * static_cast<uint32_t>(alignment);

		//	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.pipelineLayout, 1, 1, &initUniform->GetDescriptorSets()[1][currentFrame], 1, &dynamicOffset);
		//	drawObject(commandBuffer, m_objects[index].vertices.GetVertexBuffer(), m_objects[index].indices.GetIndexBuffer(),
		//		m_objects[index].indices.GetIndicies());
		//}

	}

};