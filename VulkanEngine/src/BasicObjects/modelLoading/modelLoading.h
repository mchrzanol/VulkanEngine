#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include "Libraries.h"
#include "Renderer/VertexIndexBuffer.h"

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}


class ENGINE_API modelLoading {
private:

public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	void loadModel(std::string modelPath, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool CommandPool);
};