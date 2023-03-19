#include "Triangle.h"

void triangle::create(glm::vec3 origin, float size, glm::vec3 color[3]) {
	float h = (size * std::sqrt(3) / 2);

	verticesInfo[0] = { { origin.x - size * 0.5, origin.y - h * 0.5, 0.0f }, color[0] };//left
	verticesInfo[1] = { {origin.x, origin.y + h * 0.5, 0.0f }, color[1]};//right
	verticesInfo[2] = { { origin.x + size / 2, origin.y - h * 0.5, 0.0f }, color[2]};//top

	m_Vertices.createVertexBuffer(verticesInfo);

	m_Indices.createIndexBuffer(Indices);
}

void triangle::create(glm::vec3 vertices[3], glm::vec3 color[3]) {
	this->origin = { (vertices[0].x + vertices[1].x + vertices[2].x) / 3, (vertices[0].y + vertices[1].y + vertices[2].y) / 3, (vertices[0].z + vertices[1].z + vertices[2].z) / 2 };

	verticesInfo[0] = { vertices[0], color[0]};
	verticesInfo[1] = { vertices[1], color[1]};
	verticesInfo[2] = { vertices[2], color[2]};

	m_Vertices.createVertexBuffer(verticesInfo);

	m_Indices.createIndexBuffer(Indices);
}