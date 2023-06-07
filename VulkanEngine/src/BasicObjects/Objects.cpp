#include "BasicObjects/Objects.h"

void Objects::createBuffers() {
	VertexBuffer RectangleVertexBuffer;
	VertexBuffer TriangleVertexBuffer;

	glm::vec3 testVertex[4] = { {-0.5f, -0.5f, 0.0f} , {0.5f, -0.5, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.f} };
	glm::vec3 color[4] = { {1.0f, 1.0f, 1.0f} , {1.0f, 1.0f, 1.0f} ,{1.0f, 1.0f, 1.0f}, {1,1,1} };

	std::vector<Vertex> verticesinfo(4);

	verticesinfo[0] = { testVertex[0], color[0], {1.0f, 1.0f} };
	verticesinfo[1] = { testVertex[1], color[1], {0.0f, 1.0f} };
	verticesinfo[2] = { testVertex[2], color[2], {0.0f, 0.0f} };
	verticesinfo[3] = { testVertex[3], color[3], {1.0f, 0.0f} };


	RectangleVertexBuffer.createVertexBuffer(verticesinfo, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, VulkanCore->m_Hardwaredevice.graphicsQueue, *CommandPool);

	EntitiesVertexBuffer["Rectangle"] = RectangleVertexBuffer;

	glm::vec3 testVertex2[3] = { {-0.5f, -0.5f, 0.0f} , {0.5f, -0.5, 0.0f}, {0,std::sqrt(3) / 2 - 0.5f, 0} };
	glm::vec3 color2[3] = { {1.0f, 1.0f, 1.0f} , {1.0f, 1.0f, 1.0f} ,{1.0f, 1.0f, 1.0f} };

	std::vector<Vertex> verticesinfo2(3);

	verticesinfo2[0] = { testVertex2[0], color2[0], {-1,-1} };
	verticesinfo2[1] = { testVertex2[1], color2[1], {-1,-1} };
	verticesinfo2[2] = { testVertex2[2], color2[2], {-1,-1} };

	TriangleVertexBuffer.createVertexBuffer(verticesinfo2, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, VulkanCore->m_Hardwaredevice.graphicsQueue, *CommandPool);
	
	EntitiesVertexBuffer["Triangle"] = TriangleVertexBuffer;

	VkDeviceSize bufferSize = sizeof(VkDrawIndirectCommand) * m_data.MaximumObjectsOnFrame;

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, DrawCommandBuffer, DrawCommandBufferMemory, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice);

	vkMapMemory(VulkanCore->device, DrawCommandBufferMemory, 0, bufferSize, 0, &DrawCommandData);

	model_vertexBuffer.createVertexBuffer(model.vertices, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, VulkanCore->m_Hardwaredevice.graphicsQueue, *CommandPool);
	model_IndexBuffer.createIndexBuffer(model.indices, VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, VulkanCore->m_Hardwaredevice.graphicsQueue, *CommandPool);
}

void Objects::addTexture(std::string NameOfTexture, std::string path) {
	if (auto search = texture.textures.find(NameOfTexture); search != texture.textures.end()) {
		std::cout << "That name is used. Type diffrent.\n";

		return;
	}
	std::cout << "Avaiable texture slots: " << m_data.MaximumTextures - texture.textures.size()-1 << '\n';
	if (texture.textures.size() == m_data.MaximumTextures) {
		std::cout << "To many textures has been load. Delete some to add new." << '\n';

		return;
	}

	texture.createTextureImage(VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, *CommandPool, VulkanCore->m_Hardwaredevice.graphicsQueue, NameOfTexture, path);
	texture.createTextureImageView(VulkanCore->device, NameOfTexture);


	updateTextures();

	delete[] drawCommands;
	drawCommands = updateDrawCommand();

	textureIndexingChanged = true;
}

void Objects::updateTextures() {
	initUniform->updateBasicTextureLoaded(VulkanCore->device, VulkanCore->m_Hardwaredevice.physicalDevice, texture.textureSampler, texture.textures, texture.glitchedTexture);

	std::cout << "Textures have been updated.\n";
}

void Objects::deleteTexture(std::string nameOfTexture) {
	if (auto search = texture.textures.find(nameOfTexture); search != texture.textures.end()) {
		texture.textures.erase(nameOfTexture);

		std::cout << "Texture " << nameOfTexture << " has been deleted.\n";
	}
	else
		std::cout << "Gived texture doesn't exist.\n";
}

void Objects::clearAllTextures() {
	for (auto it = texture.textures.begin(); it != texture.textures.end();)
	{
		texture.textures.erase(it);
	}

	std::cout << "All textured have been deleted.\n";
}

void Objects::compact_draw(int index) {

		if (auto search = indexOfDraw.find(m_objects[index].ID); search != indexOfDraw.end()) {
			batchDraw[search->second].count++;
			batchDraw[search->second].model.push_back(m_objects[index].model);
			batchDraw[search->second].color.push_back(m_objects[index].color);
			batchDraw[search->second].textureName.push_back(m_objects[index].textureName);

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
			newDraw.textureName.push_back(m_objects[index].textureName);

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
			EntityUBO[i].color = glm::vec3(1, 1, 1);// draw.color[i - draw.first];
			
			if (draw.textureName[i - draw.first] == "") {
				textureIndexing[i] = -1;
			}
			else {
				if (auto search = texture.textures.find(draw.textureName[i - draw.first]); search != texture.textures.end()) {
					textureIndexing[i] = texture.textures[draw.textureName[i - draw.first]].index;
				}
				else {
					textureIndexing[i] = 0;
				}
			}
		}
	}

	VkDeviceSize bufferSize = sizeof(VkDrawIndirectCommand) * m_stats.EntitiesCount;

	memcpy(DrawCommandData, drawCommand, (size_t)bufferSize);

	return drawCommand;
}

void Objects::updateUniforms(VkCommandBuffer& commandBuffer, uint32_t currentFrame) {
	UniformBufferObject ubo = { viewMatrix, projMatrix };
	initUniform->updateStaticUniformBuffer(0, TypeOfUniform::GlobalUniform, ubo, currentFrame);

	if (EntityUBOchanged == true) {//frame 0 and 1
		initUniform->updateArrayUniformBuffer(0, TypeOfUniform::EntityUniform, EntityUBO, 0, m_stats.EntitiesCount, initUniform->GetAlignment(sizeof(modelUBO), VulkanCore->m_Hardwaredevice.physicalDevice));
		initUniform->updateArrayUniformBuffer(0, TypeOfUniform::EntityUniform, EntityUBO, 1, m_stats.EntitiesCount, initUniform->GetAlignment(sizeof(modelUBO), VulkanCore->m_Hardwaredevice.physicalDevice));
		EntityUBOchanged = false;
	}
	if (textureIndexingChanged == true) {//frame 0 and 1
		initUniform->updateArrayUniformBuffer(0, TypeOfUniform::EntityUniform, textureIndexing, 2, m_stats.EntitiesCount, initUniform->GetAlignment(sizeof(int), VulkanCore->m_Hardwaredevice.physicalDevice));
		initUniform->updateArrayUniformBuffer(0, TypeOfUniform::EntityUniform, textureIndexing, 3, m_stats.EntitiesCount, initUniform->GetAlignment(sizeof(int), VulkanCore->m_Hardwaredevice.physicalDevice));
		textureIndexingChanged = false;
	}

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.graphicsPipeline["BasicShader"].pipelineLayout, 0, 1, &initUniform->GetDescriptorSets()[0][currentFrame], 0, nullptr);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.graphicsPipeline["BasicShader"].pipelineLayout, 1, 1, &initUniform->GetDescriptorSets()[1][currentFrame], 0, nullptr);
}

void Objects::drawIndirect(VkCommandBuffer& commandBuffer, uint32_t currentFrame) {

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.graphicsPipeline["BasicShader"].pipeline);

	updateUniforms(commandBuffer, currentFrame);

	for (IndirectBatch& draw : batchDraw) {
		VkBuffer vertexBuffers[] = { EntitiesVertexBuffer[draw.ID].GetVertexBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		VkDeviceSize indirect_offset = draw.first * sizeof(VkDrawIndirectCommand);
		uint32_t draw_stride = sizeof(VkDrawIndirectCommand);

		//execute the draw command buffer on each section as defined by the array of draws
		vkCmdDrawIndirect(commandBuffer, DrawCommandBuffer, indirect_offset, draw.count, draw_stride);
	}

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.graphicsPipeline["ModelShader"].pipeline);

	VkBuffer vertexBuffers[] = { model_vertexBuffer.GetVertexBuffer() };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer, model_IndexBuffer.GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32); 

	UniformBufferObject ubo = { viewMatrix, projMatrix };
	initUniform->updateModelStaticUniformBuffer(0, TypeOfUniform::GlobalUniform, ubo, currentFrame);

	modelUBO model = { glm::scale(glm::mat4(1.f), glm::vec3(0.5f)), glm::vec3(1)};

	initUniform->updateModelArrayUniformBuffer(0, TypeOfUniform::EntityUniform, model, currentFrame, 1, sizeof(modelUBO));

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.graphicsPipeline["ModelShader"].pipelineLayout, 0, 1, &initUniform->GetModelDescriptorSets()[0][currentFrame], 0, nullptr);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanCore->m_Pipeline.graphicsPipeline["ModelShader"].pipelineLayout, 1, 1, &initUniform->GetModelDescriptorSets()[1][currentFrame], 0, nullptr);

	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(model_IndexBuffer.GetIndicies().size()), 1, 0, 0, 0);
}