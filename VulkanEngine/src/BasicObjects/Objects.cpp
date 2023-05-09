#include "BasicObjects/Objects.h"

void Objects::createBuffers() {
	glm::vec3 testVertex[4] = { {-0.5f, -0.5f, 0.0f} , {0.5f, -0.5, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.f} };
	glm::vec3 color[4] = { {1.0f, 1.0f, 1.0f} , {1.0f, 1.0f, 1.0f} ,{1.0f, 1.0f, 1.0f}, {1,1,1} };

	std::vector<Vertex> verticesinfo(4);

	verticesinfo[0] = { testVertex[0], color[0] };
	verticesinfo[1] = { testVertex[1], color[1] };
	verticesinfo[2] = { testVertex[2], color[2] };
	verticesinfo[3] = { testVertex[3], color[3] };


	RectangleVertexBuffer.createVertexBuffer(verticesinfo, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, VulkanCore->m_Hardwaredevice.graphicsQueue, *CommandPool);

	glm::vec3 testVertex2[3] = { {-0.5f, -0.5f, 0.0f} , {0.5f, -0.5, 0.0f}, {0,std::sqrt(3) / 2 - 0.5f, 0} };
	glm::vec3 color2[3] = { {1.0f, 1.0f, 1.0f} , {1.0f, 1.0f, 1.0f} ,{1.0f, 1.0f, 1.0f} };

	std::vector<Vertex> verticesinfo2(4);

	verticesinfo2[0] = { testVertex2[0], color2[0] };
	verticesinfo2[1] = { testVertex2[1], color2[1] };
	verticesinfo2[2] = { testVertex2[2], color2[2] };

	TriangleVertexBuffer.createVertexBuffer(verticesinfo2, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, VulkanCore->m_Hardwaredevice.graphicsQueue, *CommandPool);
	
	std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };
	RectangleIndexBuffer.createIndexBuffer(indices, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, VulkanCore->m_Hardwaredevice.graphicsQueue, * CommandPool);
	
	std::vector<uint16_t> indices2 = { 0, 1, 2 };
	TriangleIndexBuffer.createIndexBuffer(indices2, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, VulkanCore->m_Hardwaredevice.graphicsQueue, *CommandPool);
	
	VkDeviceSize bufferSize = sizeof(VkDrawIndexedIndirectCommand) * m_data.MaximumObjectsOnFrame;

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, DrawCommandBuffer, DrawCommandBufferMemory, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice);

	vkMapMemory(VulkanCore->device, DrawCommandBufferMemory, 0, bufferSize, 0, &DrawCommandData);
}

void Objects::updateDrawCommand(VkDrawIndexedIndirectCommand*& drawCommand) {
	drawCommand = new VkDrawIndexedIndirectCommand[m_stats.EntitiesCount];

	for (int i = 0; i < m_stats.EntitiesCount; i++)
	{
		drawCommand[i].indexCount = 6;
		drawCommand[i].instanceCount = 1;
		drawCommand[i].firstIndex = 0;
		drawCommand[i].vertexOffset = 0;
		drawCommand[i].firstInstance = i; //used to access object matrix in the shader

		model[i] = m_objects[i].model;
	}

	VkDeviceSize bufferSize = sizeof(VkDrawIndexedIndirectCommand) * m_stats.EntitiesCount;

	memcpy(DrawCommandData, drawCommand, (size_t)bufferSize);
}

void Objects::updateUniforms(VkCommandBuffer& commandBuffer, uint32_t currentFrame) {
	UniformBufferObject ubo = { viewMatrix, projMatrix };
	initUniform->updateStaticUniformBuffer(0, TypeOfUniform::GlobalUniform, ubo, currentFrame);
	initUniform->updateArrayUniformBuffer(0, TypeOfUniform::EntityUniform, model, currentFrame, m_stats.EntitiesCount, initUniform->GetAlignment(sizeof(glm::mat4), VulkanCore->m_Hardwaredevice.physicalDevice));

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.pipelineLayout, 0, 1, &initUniform->GetDescriptorSets()[0][currentFrame], 0, nullptr);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.pipelineLayout, 1, 1, &initUniform->GetDescriptorSets()[1][currentFrame], 0, nullptr);
}

void Objects::drawIndirect(VkCommandBuffer& commandBuffer, uint32_t currentFrame) {

	VkDrawIndexedIndirectCommand* drawCommands;

	updateDrawCommand(drawCommands);

	updateUniforms(commandBuffer, currentFrame);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.graphicsPipeline["BasicShader"]);

	VkBuffer vertexBuffers[] = { RectangleVertexBuffer.GetVertexBuffer()};
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer, RectangleIndexBuffer.GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);//uint16 as type of variable

	VkDeviceSize indirect_offset = 0 * sizeof(VkDrawIndirectCommand);
	uint32_t draw_stride = sizeof(VkDrawIndexedIndirectCommand);

	//execute the draw command buffer on each section as defined by the array of draws
	vkCmdDrawIndexedIndirect(commandBuffer, DrawCommandBuffer, indirect_offset, m_stats.EntitiesCount, draw_stride);
}