#include "Rectangle.h"

void rectangle::create(glm::vec3 origin, float size, glm::vec3 color[4]) {

	this->origin = origin;

	verticesInfo[0] = { glm::vec3(origin.x - size * 0.5, origin.y - size * 0.5, origin.z), color[0] };//left-bottom
	std::cout << "verticesInfo[0]: " << verticesInfo[0].pos.x << " " << verticesInfo[0].pos.y << " " << verticesInfo[0].pos.z << std::endl;

	verticesInfo[1] = { glm::vec3(origin.x + size * 0.5, origin.y - size * 0.5, origin.z), color[1] };//right-bottom
	std::cout << "verticesInfo[1]: " << verticesInfo[1].pos.x << " " << verticesInfo[1].pos.y << " " << verticesInfo[1].pos.z << std::endl;

	verticesInfo[2] = { glm::vec3(origin.x + size * 0.5, origin.y + size * 0.5, origin.z), color[2] };//right-top
	std::cout << "verticesInfo[2]: " << verticesInfo[2].pos.x << " " << verticesInfo[2].pos.y << " " << verticesInfo[2].pos.z << std::endl;

	verticesInfo[3] = { glm::vec3(origin.x - size * 0.5, origin.y + size * 0.5, origin.z), color[3] };//left-top
	std::cout << "verticesInfo[3]: " << verticesInfo[3].pos.x << " " << verticesInfo[3].pos.y << " " << verticesInfo[3].pos.z << std::endl;

	m_Vertices->createVertexBuffer(verticesInfo);

	m_Indices->createIndexBuffer(Indices);
}

void rectangle::create(glm::vec3 origin, float sizeHorrizontal, float sizeVertical, glm::vec3 color[4]) {

	this->origin = origin;

	verticesInfo[0] = { glm::vec3(origin.x - sizeHorrizontal * 0.5, origin.y - sizeVertical * 0.5, origin.z), color[0] };//left-bottom
	std::cout << "verticesInfo[0]: " << verticesInfo[0].pos.x << " " << verticesInfo[0].pos.y << " " << verticesInfo[0].pos.z << std::endl;

	verticesInfo[1] = { glm::vec3(origin.x + sizeHorrizontal * 0.5, origin.y - sizeVertical * 0.5, origin.z), color[1] };//right-bottom
	std::cout << "verticesInfo[1]: " << verticesInfo[1].pos.x << " " << verticesInfo[1].pos.y << " " << verticesInfo[1].pos.z << std::endl;

	verticesInfo[2] = { glm::vec3(origin.x + sizeHorrizontal * 0.5, origin.y + sizeVertical * 0.5, origin.z), color[2] };//right-top
	std::cout << "verticesInfo[2]: " << verticesInfo[2].pos.x << " " << verticesInfo[2].pos.y << " " << verticesInfo[2].pos.z << std::endl;

	verticesInfo[3] = { glm::vec3(origin.x - sizeHorrizontal * 0.5, origin.y + sizeVertical * 0.5, origin.z), color[3] };//left-top
	std::cout << "verticesInfo[3]: " << verticesInfo[3].pos.x << " " << verticesInfo[3].pos.y << " " << verticesInfo[3].pos.z << std::endl;

	m_Vertices->createVertexBuffer(verticesInfo);

	m_Indices->createIndexBuffer(Indices);
}

void rectangle::create(glm::vec3 vertices[4], glm::vec3 color[4]) {
	this->origin = { (vertices[0].x + vertices[1].x + vertices[2].x) / 3, (vertices[0].y + vertices[1].y + vertices[2].y) / 3, (vertices[0].z + vertices[1].z + vertices[2].z) / 2 };

	verticesInfo[0] = { vertices[0], color[0] };
	std::cout << "verticesInfo[0]: " << verticesInfo[0].pos.x << " " << verticesInfo[0].pos.y << " " << verticesInfo[0].pos.z << std::endl;

	verticesInfo[1] = { vertices[1], color[1] };
	std::cout << "verticesInfo[1]: " << verticesInfo[1].pos.x << " " << verticesInfo[1].pos.y << " " << verticesInfo[1].pos.z << std::endl;

	verticesInfo[2] = { vertices[2], color[2] };
	std::cout << "verticesInfo[2]: " << verticesInfo[2].pos.x << " " << verticesInfo[2].pos.y << " " << verticesInfo[2].pos.z << std::endl;

	verticesInfo[3] = { vertices[3], color[3] };
	std::cout << "verticesInfo[3]: " << verticesInfo[3].pos.x << " " << verticesInfo[3].pos.y << " " << verticesInfo[3].pos.z << std::endl;

	m_Vertices->createVertexBuffer(verticesInfo);

	m_Indices->createIndexBuffer(Indices);
}