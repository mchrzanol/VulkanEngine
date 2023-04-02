#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"

extern GlobalUtl utils;

class ENGINE_API rectangle {
private:
	glm::vec3 origin;
	float size;

	std::vector<Vertex> verticesInfo;

	std::vector<uint16_t> Indices;
public:
	rectangle() {
		verticesInfo.resize(4);
		Indices.insert(Indices.end(), { 0,1,2, 2, 3, 0 });
	}

	~rectangle() {
		verticesInfo.clear();
		Indices.clear();
	}

	void create(glm::vec3 origin, float size, glm::vec3 color[4]);
	void create(glm::vec3 origin, float sizeHorrizontal, float sizeVertical, glm::vec3 color[4]);
	void create(glm::vec3 vertices[4], glm::vec3 color[4]);

	void update();

	inline std::vector<Vertex>& GetVertices() { return verticesInfo; };
	inline std::vector<uint16_t>& GetIndices() { return Indices; };

	inline glm::vec3 GetOrigin() { return origin; }
};