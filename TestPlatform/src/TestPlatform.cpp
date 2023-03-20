#include "TestPlatform.h"
#include "Keys.h"


void TestPlatform::run() {
	Init();
	mainLoop();
	CleanUp();
}

void TestPlatform::Init() {
	
	window = new windowClass;

	window->initWindow("TestPlatform");
	initInput = new Input(window->m_window);
	window->addUserPointer(initInput);

	initUniform = new UniformBuffer(MAX_FRAMES_IN_FLIGHT);
	initVertices = new VertexBuffer();
	initIndices = new IndexBuffer();


	initVulkan = new VulkanClass(window->m_window, initUniform);
	initVulkan->init();

	initCommandPool = new CommandPool(initVulkan, initUniform, window, MAX_FRAMES_IN_FLIGHT);
	initCommandPool->createCommandPool();

	triangle* object1 = new triangle();
	glm::vec3 color[3] = { {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f, 0.0f} ,{0.0f, 0.0f, 1.0f} };
	glm::vec3 testVertex[3] = { {-0.5f, -0.433013f, 0.0f} , {0.5f, -0.433013f, 0.0f}, {0.5f, 0.433013f, 0.0f} };

	//object1->create(testVertex, color);
	object1->create(glm::vec3(0.0, 0.0, 0.0), 1.f, color);


	objects.m_triangles.push_back(*object1);













	 std::vector<Vertex> vertices = {
{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
	};



	initVertices->createVertexBuffer(vertices);

	 std::vector<uint16_t> indices = {
0, 1, 2, 2, 3, 0
	};
	initIndices->createIndexBuffer(indices);

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
		initCommandPool->drawFrame(objects.m_triangles[0].GetVertexBuffer(), objects.m_triangles[0].GetIndexBuffer(), objects.m_triangles[0].GetIndices());

		if (initInput->keyState[KEY_ESCAPE].press == true) {
			break;
		}
		
		initInput->ClearKeyState();
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