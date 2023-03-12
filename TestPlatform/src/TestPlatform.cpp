#include "TestPlatform.h"

void TestPlatform::run() {
	Init();
	mainLoop();
	CleanUp();
}

void TestPlatform::Init() {

	window = new windowClass;
	window->initWindow("TestPlatform", 1080, 980);

	initUniform = new UniformBuffer(MAX_FRAMES_IN_FLIGHT);
	initVertices = new VertexBuffer();
	initIndices = new IndexBuffer();

	initVulkan = new VulkanClass(window->m_window, initUniform);
	initVulkan->init();

	initCommandPool = new CommandPool(initVulkan, initUniform, initIndices, initVertices, window, MAX_FRAMES_IN_FLIGHT);
	initCommandPool->createCommandPool();

	initVertices->createVertexBuffer(initVulkan->GetDevice(), initCommandPool->GetCommandPool(), initVulkan->GetGraphicsQueue(), initVulkan->GetPhyscicalDevice());
	initIndices->createIndexBuffer(initVulkan->GetDevice(), initCommandPool->GetCommandPool(), initVulkan->GetGraphicsQueue(), initVulkan->GetPhyscicalDevice());
	initUniform->createUnifromBuffer(initVulkan->GetDevice(), initVulkan->GetPhyscicalDevice());
	
	initUniform->createDescriptorPool(initVulkan->GetDevice());
	initUniform->createDescriptorSets(initVulkan->GetDevice());

	initCommandPool->createCommandBuffers();
	initCommandPool->createSyncObjects();
}

void TestPlatform::mainLoop() {
	while (!window->isWindowClosed()) {
		std::cout << window->m_Data.Height << " " << window->m_Data.Width << std::endl;
		window->PoolEvents();
		initCommandPool->drawFrame();
	}

}

void TestPlatform::CleanUp() {
	window->destroyWindow();
}