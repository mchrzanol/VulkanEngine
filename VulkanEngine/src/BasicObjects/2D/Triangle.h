#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"
#include "BasicObjects/Entity.h"

namespace Entity {
	namespace triangle {
		ENGINE_API EntityVitalInfo* create(glm::vec3 origin, float size, Orientation orientation, glm::vec3 color = glm::vec3(1,1,1));
	}
}