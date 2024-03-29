#include "Rectangle.h"
namespace Entity {
	namespace rectangle {
		ENGINE_API EntityVitalInfo* create(glm::vec3 origin, float size, Orientation orientation, glm::vec3 color) {
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

			data->color = color;
		
			data->vertexCount = 4;

			data->ID = "Rectangle";

			return data;
		}


		EntityVitalInfo* create(glm::vec3 origin, float sizeHorrizontal, float sizeVertical, Orientation orientation, glm::vec3 color) {
			EntityVitalInfo* data = new EntityVitalInfo();

			data->origin = origin;

			data->model = glm::translate(data->model, origin);

			data->model = glm::scale(data->model, glm::vec3(sizeHorrizontal,sizeVertical, 1));

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

			data->color = color;

			data->vertexCount = 4;

			data->ID = "Rectangle";

			return data;
		}
		ENGINE_API EntityVitalInfo* create(glm::vec3 origin, float size, Orientation orientation, std::string NameOfTexture) {
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

			data->textureName = NameOfTexture;

			data->vertexCount = 4;

			data->ID = "Rectangle";

			return data;
		}


		EntityVitalInfo* create(glm::vec3 origin, float sizeHorrizontal, float sizeVertical, Orientation orientation, std::string NameOfTexture) {
			EntityVitalInfo* data = new EntityVitalInfo();

			data->origin = origin;

			data->model = glm::translate(data->model, origin);

			data->model = glm::scale(data->model, glm::vec3(sizeHorrizontal, sizeVertical, 1));

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

			data->textureName = NameOfTexture;

			data->vertexCount = 4;

			data->ID = "Rectangle";

			return data;
		}
	}
}