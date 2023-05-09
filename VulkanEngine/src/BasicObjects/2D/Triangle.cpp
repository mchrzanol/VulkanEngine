#include "Triangle.h"

namespace Entity {
	namespace triangle {
		EntityVitalInfo* create(glm::vec3 origin, float size) {
		
			EntityVitalInfo* data = new EntityVitalInfo();

			data->indexCount = 3;

			data->origin = origin;

			data->model = glm::translate(data->model, origin);

			data->model = glm::scale(data->model, glm::vec3(size));

			data->data.type = EntityType::Triangle;

			return data;
		}
	}
}