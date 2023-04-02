#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"

extern GlobalUtl utils;

class ENGINE_API triangle {
private:
	glm::vec3 origin;
	float size;

	std::vector<Vertex> verticesInfo;

	std::vector<uint16_t> Indices;
public:
	triangle() {
		verticesInfo.resize(3);
		Indices.insert(Indices.end(), { 0,1,2 });
	}

	~triangle() {
		verticesInfo.clear();
		Indices.clear();
	}
	void create(glm::vec3 origin, float size, glm::vec3 color[3]);//glm::vec3 orientation;
	void create(glm::vec3 vertices[3], glm::vec3 color[3]);//glm::vec3 orientation;

	void update() ;

	inline std::vector<Vertex>& GetVertices() { return verticesInfo; };
	inline std::vector<uint16_t>& GetIndices() { return Indices; };

	inline glm::vec3 GetOrigin() { return origin; }

};