#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"

extern GlobalUtl utils;

struct TriangleInfo {
	glm::vec3 orientation;//later
	
	//equilater/
	glm::vec3 origin;
	float size;

	//other
	glm::vec3 customVertex[3];
};

class ENGINE_API triangle {
private:
	glm::vec3 origin;
	float size;

	std::vector<Vertex> verticesInfo;

	std::vector<uint16_t> Indices;

	VertexBuffer m_Vertices;
	IndexBuffer m_Indices;
public:
	triangle() {
		verticesInfo.resize(3);
		Indices.insert(Indices.end(), { 0,1,2 });
	}
	void create(glm::vec3 origin, float size, glm::vec3 color[3]);
	void create(glm::vec3 vertices[3], glm::vec3 color[3]);

	void update() ;

	void destroy() ;
};