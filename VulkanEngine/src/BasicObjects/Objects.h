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

struct UniformBufferObject {
	alignas(16) glm::mat4 model;//alignas(16) = przesuniecie bajtowe(co 16 bajtów)
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
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

	UniformBuffer* initUniform;

	//Data m_data;
	stats m_stats;

	std::vector<Entity> m_objects;
	std::map<uint32_t, EntityData> m_data;

	//line
	//circle

	//test
	glm::mat4 * models = nullptr;
	size_t alignment;

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;



public:
	Objects(int MAX_FRAMES_IN_FLIGHT)
	{
		initUniform = new UniformBuffer(MAX_FRAMES_IN_FLIGHT, m_stats.MaximumObjectsOnFrame);

		initUniform->createUniformBind(0, sizeof(UniformBufferObject), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, TypeOfUniform::GlobalUniform);
		initUniform->createUniformBind(0, sizeof(glm::mat4), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, TypeOfUniform::EntityUniform);

		//for(int i = 0;i< MAX_FRAMES_IN_FLIGHT;i++)
		//	initUniform->updateDynamicUniformBuffer(0, TypeOfUniform::LocalUniform, models, 0);
		//initUniform->updateDynamicUniformBuffer(0, TypeOfUniform::LocalUniform, models, 1);
	}

	void Init() {
		m_objects.reserve(m_stats.MaximumObjectsOnFrame);
		m_stats.EntitiesCount = 0;

		viewMatrix = glm::mat4(1.f);
		projMatrix = glm::mat4(1.f);

		alignment = initUniform->GetAlignment(sizeof(glm::mat4));
		size_t bufferSize = m_stats.MaximumObjectsOnFrame * alignment;
		models = (glm::mat4*)_aligned_malloc(bufferSize, alignment);

	}

	template<typename T>
	void PushBack(T * object) {
		throw std::runtime_error("Undefined Object!");
	}

	template<>
	void PushBack<triangle>(triangle * object) {

		m_objects.push_back(Entity());
		m_objects[m_stats.EntitiesCount].createVertexBuffer(object->GetVertices());
		m_objects[m_stats.EntitiesCount].createIndexBuffer(object->GetIndices());

		m_data[m_stats.EntitiesCount] = { EntityType::Triangle };

		models[m_stats.EntitiesCount] = glm::mat4(1.f);

		m_stats.EntitiesCount++;
		m_stats.triangleCount++;

	}

	template<>
	void PushBack<rectangle>(rectangle * object) {
		m_objects.push_back(Entity());
		m_objects[m_stats.EntitiesCount].createVertexBuffer(object->GetVertices());
		m_objects[m_stats.EntitiesCount].createIndexBuffer(object->GetIndices());

		m_data[m_stats.EntitiesCount] = { EntityType::Rectangle };

		models[m_stats.EntitiesCount] = glm::mat4(1.f);

		m_stats.EntitiesCount++;
		m_stats.rectangleCount++;
	}

	void UpdateView(glm::mat4 viewMatrix) {
		this->viewMatrix = viewMatrix;
	}

	void UpdateProjection(glm::mat4 projMatrix) {
		this->projMatrix = projMatrix;
	}

	void draw2DObjects(VkCommandBuffer & commandBuffer, VkPipelineLayout pipelineLayout, uint32_t currentFrame) {

		UniformBufferObject ubo = { glm::mat4(1.f), viewMatrix, projMatrix };
		initUniform->updateStaticUniformBuffer(0, TypeOfUniform::GlobalUniform, ubo, currentFrame);

		initUniform->updateDynamicUniformBuffer(0, TypeOfUniform::EntityUniform, models, currentFrame, m_stats.EntitiesCount);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &initUniform->GetDescriptorSets()[0][currentFrame], 0, nullptr);
		for (size_t index = 0; index < m_stats.EntitiesCount; index++) {
			uint32_t dynamicOffset = index * static_cast<uint32_t>(alignment);

			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 1, 1, &initUniform->GetDescriptorSets()[1][currentFrame], 1, &dynamicOffset);
			drawObject(commandBuffer, m_objects[index].vertices.GetVertexBuffer(), m_objects[index].indices.GetIndexBuffer(),
				m_objects[index].indices.GetIndicies(), pipelineLayout);
		}

	}

	void destroy() {
		m_objects.clear();
		initUniform->cleanup();
	}

	inline int GetTrianglesSize() { return m_stats.triangleCount; };
	inline int GetRectanglesSize() { return m_stats.rectangleCount; };

	inline UniformBuffer* GetUniformBuffer() { return initUniform; };

		//cos takiego jak przy registerclass

};