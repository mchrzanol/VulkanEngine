#pragma once
#include "2D/Triangle.h"
#include "2D/Rectangle.h"
#include "VulkanStructure/VulkanStructure.h"

#include "BasicObjects/TextureImage/texture_Image.h"
#include "BasicObjects/modelLoading/modelLoading.h"

const static struct data {
	const static uint32_t MaximumObjectsOnFrame = 10000;
	const static uint32_t MaximumTextures = 20;

	const std::string BasicShaders[2] = {
		"../shaders/BasicVert.spv",
		"../shaders/BasicFrag.spv"
	};

	const std::string CircleShaders[2] = {
		"",
		""
	};

	const std::string ModelShaders[2] = {
		"../shaders/ModelVert.spv",
		"../shaders/ModelFrag.spv"
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
		std::vector<std::string> textureName;
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

	//textures
	texturesLoading texture;
	int textureIndexing[m_data.MaximumObjectsOnFrame];
	bool textureIndexingChanged = true;

	//modeltest
	std::string MODEL_PATH = "../models/doritos.obj";
	std::string TEXTURE_PATH = "../textures/doritos.png";

	textureData modelTexture;

	modelLoading model;

	VertexBuffer model_vertexBuffer;
	IndexBuffer model_IndexBuffer;

public:
	Objects(int MAX_FRAMES_IN_FLIGHT) 
	{
		initUniform = new UniformBuffer(MAX_FRAMES_IN_FLIGHT, m_data.MaximumObjectsOnFrame, m_data.MaximumTextures);
	}

	void Init(VulkanStruct* Vulkan, VkCommandPool* CommandPool) {
		VulkanCore = Vulkan;

		this->CommandPool = CommandPool;

		m_objects.reserve(m_data.MaximumObjectsOnFrame);
		m_stats.EntitiesCount = 0;

		viewMatrix = glm::mat4(1.f);
		projMatrix = glm::mat4(1.f);

		VulkanCore->m_Pipeline.createGraphicsPipeline(this->VulkanCore->device, "BasicShader", m_data.BasicShaders[0], m_data.BasicShaders[1], initUniform->descriptorSetLayouts, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN);
		VulkanCore->m_Pipeline.createGraphicsPipeline(this->VulkanCore->device, "ModelShader", m_data.ModelShaders[0], m_data.ModelShaders[1], initUniform->model_descriptorSetLayouts, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

		texture.createTextureSampler(VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice);

		initUniform->createUniformBuffers(Vulkan->device, Vulkan->m_Hardwaredevice.physicalDevice);
		initUniform->createDescriptorPool(Vulkan->device);

		texture.glitchedTexture = texture.createIndependentTexture(VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, *CommandPool, VulkanCore->m_Hardwaredevice.graphicsQueue, "../textures/glitched.jpg");

		initUniform->createBasicDescriptorSets(Vulkan->device, Vulkan->m_Hardwaredevice.physicalDevice, texture.textureSampler, texture.textures, texture.glitchedTexture);

		//test-model
		modelTexture = texture.createIndependentTexture(VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, *CommandPool, VulkanCore->m_Hardwaredevice.graphicsQueue, TEXTURE_PATH);
		initUniform->createModelDescriptorSets(Vulkan->device, Vulkan->m_Hardwaredevice.physicalDevice, texture.textureSampler, modelTexture);
		model.loadModel(MODEL_PATH, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, VulkanCore->m_Hardwaredevice.graphicsQueue, *CommandPool);
		
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

	//Textures
	void addTexture(std::string NameOftexture, std::string path);
	void deleteTexture(std::string nameOfTexture);
	void clearAllTextures();
	void updateTextures();

	void UpdateView(glm::mat4 viewMatrix) {
		this->viewMatrix = viewMatrix;
	}

	void UpdateProjection(glm::mat4 projMatrix) {
		this->projMatrix = projMatrix;
	}

	void rotate() {
		for (int i = 0; i < m_stats.EntitiesCount; i++) {//jakies rozruznianie obiektow
			EntityUBO[i].model = glm::rotate(EntityUBO[i].model, glm::radians(10.f), glm::vec3(1, 0, 0));
		}
		EntityUBOchanged = true;
	}

	void destroy() {

		texture.cleanup(VulkanCore->device);

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
	}

};