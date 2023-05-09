#include "Triangle.h"

namespace Entity {
	namespace triangle {
		EntityVitalInfo* create(glm::vec3 origin, float size, Orientation orientation) {
		
			EntityVitalInfo* data = new EntityVitalInfo();

			data->origin = origin;

			data->model = glm::translate(data->model, origin);

			data->model = glm::scale(data->model, glm::vec3(size));

			switch (static_cast<int>(orientation)) {
			case 0:
			{
				data->model = glm::rotate(data->model, glm::radians(90.f), glm::vec3(0, 1, 0));
				break;
			}
			case 1:
			{
				//data->model = glm::translate(data->model, origin);
				data->model = glm::rotate(data->model, glm::radians(90.f), glm::vec3(1, 0, 0));
				//data->model = glm::translate(data->model, origin * glm::vec3(-1.f));
				break;
			}
			}

			data->type = EntityType::Triangle;

			return data;
		}
	}
}