#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"
#include "BasicObjects/Entity.h"

namespace Entity {
	namespace rectangle {
		ENGINE_API EntityVitalInfo* create(glm::vec3 origin, float size);

		ENGINE_API EntityVitalInfo* create(glm::vec3 origin, float sizeHorrizontal, float sizeVertical);
	}
}