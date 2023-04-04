#include "Triangle.h"

namespace Entity {
	namespace triangle {
		EntityInfo* create(glm::vec3 origin, float size, glm::vec3 color[3]){
			EntityInfo* data = new EntityInfo();

			float h = (size * std::sqrt(3) / 2);

			data->origin = origin;

			std::vector<Vertex> verticesInfo(3);

			verticesInfo[0] = { glm::vec3(origin.x - size * 0.5, origin.y - h * 1 / 3, origin.z), color[0] };//left
			std::cout << "verticesInfo[0]: " << verticesInfo[0].pos.x << " " << verticesInfo[0].pos.y << " " << verticesInfo[0].pos.z << std::endl;

			verticesInfo[1] = { glm::vec3(origin.x, origin.y + h * 2 / 3, origin.z), color[1] };//top
			std::cout << "verticesInfo[1]: " << verticesInfo[1].pos.x << " " << verticesInfo[1].pos.y << " " << verticesInfo[1].pos.z << std::endl;

			verticesInfo[2] = { glm::vec3(origin.x + size * 0.5, origin.y - h * 1 / 3, origin.z), color[2] };//right
			std::cout << "verticesInfo[2]: " << verticesInfo[2].pos.x << " " << verticesInfo[2].pos.y << " " << verticesInfo[2].pos.z << std::endl;

			data->createVertexBuffer(verticesInfo);
			data->createIndexBuffer({ 0, 1, 2 });

			data->data.type = EntityType::Triangle;

			return data;
		}

		EntityInfo* create(glm::vec3 vertices[3], glm::vec3 color[3])	{
			EntityInfo* data = new EntityInfo();

			data->origin = { (vertices[0].x + vertices[1].x + vertices[2].x) / 3, (vertices[0].y + vertices[1].y + vertices[2].y) / 3, (vertices[0].z + vertices[1].z + vertices[2].z) / 2 };

			std::vector<Vertex> verticesInfo(3);

			verticesInfo[0] = { vertices[0], color[0] };
			std::cout << "verticesInfo[0]: " << verticesInfo[0].pos.x << " " << verticesInfo[0].pos.y << " " << verticesInfo[0].pos.z << std::endl;
			verticesInfo[1] = { vertices[1], color[1] };
			std::cout << "verticesInfo[1]: " << verticesInfo[1].pos.x << " " << verticesInfo[1].pos.y << " " << verticesInfo[1].pos.z << std::endl;
			verticesInfo[2] = { vertices[2], color[2] };
			std::cout << "verticesInfo[2]: " << verticesInfo[2].pos.x << " " << verticesInfo[2].pos.y << " " << verticesInfo[2].pos.z << std::endl;

			data->createVertexBuffer(verticesInfo);
			data->createIndexBuffer({ 0, 1, 2 });

			data->data.type = EntityType::Triangle;

			return data;
		}
	}
}