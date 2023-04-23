#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"

enum class EntityType {
	Triangle, Rectangle, Circle, Line
};

enum class Orientation {
	X, Y, Z
};

struct EntityData {
	EntityType type;
};

struct EntityInfo {
	VertexBuffer vertices;
	IndexBuffer indices;

	glm::vec3 origin;

	void createVertexBuffer(std::vector<Vertex>& vertices) {
		this->vertices.createVertexBuffer(vertices);
	}

	void createIndexBuffer(std::vector<uint16_t> indices) {
		this->indices.createIndexBuffer(indices);
	}

	EntityData data;

	//texid
};