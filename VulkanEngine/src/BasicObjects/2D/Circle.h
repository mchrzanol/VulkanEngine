#pragma once

#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"
#include "BasicObjects/Entity.h"

extern GlobalUtl utils;

namespace Entity {
	namespace circle {
		ENGINE_API EntityInfo* create(glm::vec3 origin, float radius,unsigned int countOfVertices, glm::vec3 color);
	}
}