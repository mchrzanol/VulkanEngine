#pragma once

#include <iostream>
#include "Engine.h"

class TestPlatform{
public:
	void run();

private:
	windowClass* window;
	VulkanClass* initVulkan;
	UniformBuffer* initUniform;
	VertexBuffer* initVertices;
	IndexBuffer* initIndices;
	CommandPool* initCommandPool;

	void Init();

	void OnUpdate();

	void OnRender();

	void mainLoop();

	void CleanUp();

	void Print();//Test
};
