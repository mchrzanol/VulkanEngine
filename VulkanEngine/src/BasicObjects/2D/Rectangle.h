#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"

extern GlobalUtl utils;

class ENGINE_API rectangle {
private:
	glm::vec3 origin;
	float size;

	glm::mat4 model;

	std::vector<Vertex> verticesInfo;

	std::vector<uint16_t> Indices;

	std::unique_ptr<VertexBuffer> m_Vertices;
	std::unique_ptr<IndexBuffer> m_Indices;
public:
	rectangle() {
		verticesInfo.resize(4);
		Indices.insert(Indices.end(), { 0,1,2, 2, 3, 0 });

		model = glm::mat4(1.f);

		m_Vertices = std::make_unique<VertexBuffer>();
		m_Indices = std::make_unique<IndexBuffer>();
	}
	void create(glm::vec3 origin, float size, glm::vec3 color[4]);
	void create(glm::vec3 origin, float sizeHorrizontal, float sizeVertical, glm::vec3 color[4]);
	void create(glm::vec3 vertices[4], glm::vec3 color[4]);

	void update();

	void destroy();

	inline VkBuffer GetVertexBuffer() { return m_Vertices->GetVertexBuffer(); };
	inline VkBuffer GetIndexBuffer() { return m_Indices->GetIndexBuffer(); };
	inline std::vector<uint16_t> GetIndices() { return m_Indices->GetIndicies(); };
};