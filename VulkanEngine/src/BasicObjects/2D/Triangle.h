#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"
#include "BasicObjects/Entity.h"

extern GlobalUtl utils;

namespace Entity {
	namespace triangle {
		ENGINE_API EntityInfo* create(glm::vec3 origin, float size, glm::vec3 color[3]);

		ENGINE_API EntityInfo* create(glm::vec3 vertices[3], glm::vec3 color[3]);
	}
}