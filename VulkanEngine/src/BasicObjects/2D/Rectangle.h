#pragma once
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"
#include "Entity.h"

extern GlobalUtl utils;

namespace Entity {
	namespace rectangle {
		ENGINE_API EntityInfo* create(glm::vec3 origin, float size, glm::vec3 color[4]);
		ENGINE_API EntityInfo* create(glm::vec3 origin, float sizeHorrizontal, float sizeVertical, glm::vec3 color[4]);
		ENGINE_API EntityInfo* create(glm::vec3 vertices[4], glm::vec3 color[4]);
	}
}