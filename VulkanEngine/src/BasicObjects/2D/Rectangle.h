#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"
#include "BasicObjects/Entity.h"

namespace Entity {
	namespace rectangle {
		ENGINE_API EntityVitalInfo* create(glm::vec3 origin, float size, Orientation orientation, glm::vec3 color = glm::vec3(1,1,1));

		ENGINE_API EntityVitalInfo* create(glm::vec3 origin, float sizeHorrizontal, float sizeVertical, Orientation orientation, glm::vec3 color = glm::vec3(1, 1, 1));
	}
}