#pragma once

#include <iostream>
#include "Engine.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

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
