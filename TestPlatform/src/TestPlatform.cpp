#include "TestPlatform.h"

void TestPlatform::run() {
	Init();
	mainLoop();
	CleanUp();
}

void TestPlatform::Init() {

	window = new windowClass;
	window->initWindow("TestPlatform");

	initUniform = new UniformBuffer(MAX_FRAMES_IN_FLIGHT);
	initVertices = new VertexBuffer();
	initIndices = new IndexBuffer();

	initVulkan = new VulkanClass(window->m_window, initUniform);
	initVulkan->init();

	initCommandPool = new CommandPool(initVulkan, initUniform, initIndices, initVertices, window, MAX_FRAMES_IN_FLIGHT);
	initCommandPool->createCommandPool();

	initVertices->createVertexBuffer();
	initIndices->createIndexBuffer();

	initUniform->createUnifromBuffer();
	
	initUniform->createDescriptorPool();
	initUniform->createDescriptorSets();

	initCommandPool->createCommandBuffers();
	initCommandPool->createSyncObjects();
}

void TestPlatform::mainLoop() {
	while (!window->isWindowClosed()) {
		//std::cout << window->m_Data.Height << " " << window->m_Data.Width << std::endl;
		window->PoolEvents();
		initCommandPool->drawFrame();
	}

	initVulkan->WaitIdle();
}

void TestPlatform::CleanUp() {

	initVulkan->cleanupSwapChain();

	initUniform->cleanup();

	initIndices->cleanup();

	initVertices->cleanup();

	initCommandPool->cleanup();

	initVulkan->cleanup();

	window->destroyWindow();
}