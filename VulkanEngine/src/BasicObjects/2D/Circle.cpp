#include "Circle.h"

namespace Entity {
	namespace circle {
		EntityInfo* create(glm::vec3 origin, float radius, unsigned int countOfVertices, glm::vec3 color) {
			EntityInfo* data = new EntityInfo();

			//data->origin = origin;

			//std::vector<Vertex> verticesInfo(countOfVertices);

			//float angle = 360 / countOfVertices;

			//float angleValue = 0;
			//for (auto& vertex : verticesInfo)
			//{
			//	vertex = { glm::vec3(origin.x + radius * cos(angleValue), origin.y + radius * sin(angleValue), origin.z), color };
			//}

			//data->createVertexBuffer(verticesInfo);
			//data->createIndexBuffer({ 0,1,2, 2, 3, 0 });

			//data->data.type = EntityType::Rectangle;

			return data;
		}
	}
}