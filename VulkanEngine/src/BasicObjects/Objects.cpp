#include "BasicObjects/Objects.h"

void Objects::createBuffers() {
	VertexBuffer RectangleVertexBuffer;
	VertexBuffer TriangleVertexBuffer;

	glm::vec3 testVertex[4] = { {-0.5f, -0.5f, 0.0f} , {0.5f, -0.5, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.f} };
	glm::vec3 color[4] = { {1.0f, 1.0f, 1.0f} , {1.0f, 1.0f, 1.0f} ,{1.0f, 1.0f, 1.0f}, {1,1,1} };

	std::vector<Vertex> verticesinfo(4);

	verticesinfo[0] = { testVertex[0], color[0] };
	verticesinfo[1] = { testVertex[1], color[1] };
	verticesinfo[2] = { testVertex[2], color[2] };
	verticesinfo[3] = { testVertex[3], color[3] };


	RectangleVertexBuffer.createVertexBuffer(verticesinfo, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, VulkanCore->m_Hardwaredevice.graphicsQueue, *CommandPool);

	EntitiesVertexBuffer["Rectangle"] = RectangleVertexBuffer;

	glm::vec3 testVertex2[3] = { {-0.5f, -0.5f, 0.0f} , {0.5f, -0.5, 0.0f}, {0,std::sqrt(3) / 2 - 0.5f, 0} };
	glm::vec3 color2[3] = { {1.0f, 1.0f, 1.0f} , {1.0f, 1.0f, 1.0f} ,{1.0f, 1.0f, 1.0f} };

	std::vector<Vertex> verticesinfo2(3);

	verticesinfo2[0] = { testVertex2[0], color2[0] };
	verticesinfo2[1] = { testVertex2[1], color2[1] };
	verticesinfo2[2] = { testVertex2[2], color2[2] };

	TriangleVertexBuffer.createVertexBuffer(verticesinfo2, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, VulkanCore->m_Hardwaredevice.graphicsQueue, *CommandPool);
	
	EntitiesVertexBuffer["Triangle"] = TriangleVertexBuffer;

	VkDeviceSize bufferSize = sizeof(VkDrawIndirectCommand) * m_data.MaximumObjectsOnFrame;

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, DrawCommandBuffer, DrawCommandBufferMemory, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice);

	vkMapMemory(VulkanCore->device, DrawCommandBufferMemory, 0, bufferSize, 0, &DrawCommandData);
}

void Objects::compact_draw(int index) {

		if (auto search = indexOfDraw.find(m_objects[index].ID); search != indexOfDraw.end()) {
			batchDraw[search->second].count++;
			batchDraw[search->second].model.push_back(m_objects[index].model);
			batchDraw[search->second].color.push_back(m_objects[index].color);

			for (int j = search->second+1; j < batchDraw.size(); j++) {
				batchDraw[j].first++;
			}
		}
		else
		{
			IndirectBatch newDraw;
			newDraw.ID = m_objects[index].ID;
			newDraw.vertexCount = m_objects[index].vertexCount;
			if (batchDraw.size() > 0)
				newDraw.first = batchDraw.back().count + batchDraw.back().first;
			else
				newDraw.first = 0;
			newDraw.count = 1;
			newDraw.model.push_back(m_objects[index].model);
			newDraw.color.push_back(m_objects[index].color);

			batchDraw.push_back(newDraw);

			indexOfDraw[m_objects[index].ID] = batchDraw.size()-1;
		}
}

VkDrawIndirectCommand* Objects::updateDrawCommand() {//do zoptymalizowania
	VkDrawIndirectCommand* drawCommand = new VkDrawIndirectCommand[m_stats.EntitiesCount];

	for (auto draw : batchDraw) {
		for (int i = draw.first; i < draw.count+draw.first; i++)
		{
			drawCommand[i].vertexCount = draw.vertexCount;
			drawCommand[i].instanceCount = 1;
			drawCommand[i].firstVertex = 0;
			drawCommand[i].firstInstance = i; //used to access object matrix in the shader

			EntityUBO[i].model = draw.model[i-draw.first];
			EntityUBO[i].color = draw.color[i - draw.first];
		}
	}

	VkDeviceSize bufferSize = sizeof(VkDrawIndirectCommand) * m_stats.EntitiesCount;

	memcpy(DrawCommandData, drawCommand, (size_t)bufferSize);

	return drawCommand;
}

void Objects::updateUniforms(VkCommandBuffer& commandBuffer, uint32_t currentFrame) {
	UniformBufferObject ubo = { viewMatrix, projMatrix };
	initUniform->updateStaticUniformBuffer(0, TypeOfUniform::GlobalUniform, ubo, currentFrame);

	if (EntityUBOchanged == true) {
		initUniform->updateArrayUniformBuffer(0, TypeOfUniform::EntityUniform, EntityUBO, 0, m_stats.EntitiesCount, initUniform->GetAlignment(sizeof(modelUBO), VulkanCore->m_Hardwaredevice.physicalDevice));
		initUniform->updateArrayUniformBuffer(0, TypeOfUniform::EntityUniform, EntityUBO, 1, m_stats.EntitiesCount, initUniform->GetAlignment(sizeof(modelUBO), VulkanCore->m_Hardwaredevice.physicalDevice));
		EntityUBOchanged = false;
	}
	//initUniform->updateArrayUniformBuffer(5, TypeOfUniform::EntityUniform, color, currentFrame, m_stats.EntitiesCount, initUniform->GetAlignment(sizeof(glm::vec3), VulkanCore->m_Hardwaredevice.physicalDevice));

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.pipelineLayout, 0, 1, &initUniform->GetDescriptorSets()[0][currentFrame], 0, nullptr);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.pipelineLayout, 1, 1, &initUniform->GetDescriptorSets()[1][currentFrame], 0, nullptr);
}

void Objects::drawIndirect(VkCommandBuffer& commandBuffer, uint32_t currentFrame) {

	updateUniforms(commandBuffer, currentFrame);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.graphicsPipeline["BasicShader"]);

	for (IndirectBatch& draw : batchDraw) {
		VkBuffer vertexBuffers[] = { EntitiesVertexBuffer[draw.ID].GetVertexBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		VkDeviceSize indirect_offset = draw.first * sizeof(VkDrawIndirectCommand);
		uint32_t draw_stride = sizeof(VkDrawIndirectCommand);

		//execute the draw command buffer on each section as defined by the array of draws
		vkCmdDrawIndirect(commandBuffer, DrawCommandBuffer, indirect_offset, draw.count, draw_stride);
	}
}