#include "Rectangle.h"
namespace Entity {
	namespace rectangle {
		ENGINE_API EntityVitalInfo* create(glm::vec3 origin, float size) {
			EntityVitalInfo* data = new EntityVitalInfo();

			data->indexCount = 6;

			data->origin = origin;

			data->model = glm::translate(data->model, origin);

			data->model = glm::scale(data->model, glm::vec3(size));
		
			data->data.type = EntityType::Rectangle;

			return data;
		}


		EntityVitalInfo* create(glm::vec3 origin, float sizeHorrizontal, float sizeVertical) {
			EntityVitalInfo* data = new EntityVitalInfo();

			data->indexCount = 6;

			data->origin = origin;

			data->model = glm::translate(data->model, origin);

			data->model = glm::scale(data->model, glm::vec3(sizeHorrizontal,sizeVertical, 0));

			data->data.type = EntityType::Rectangle;
			return data;
		}
	}
}