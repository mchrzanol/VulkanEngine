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

	//std::unique_ptr<triangle> object1 = std::make_unique<triangle>();
	glm::vec3 color[3];//= { {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f, 0.0f} ,{0.0f, 0.0f, 1.0f} };
	glm::vec3 testVertex[3] = { {-0.5f, -0.5f, 0.0f} , {0.5f, 0.5, 0.0f}, {0.5f, -0.5f, 0.0f} };
	//object1->create(testVertex, color);
	//object1->create(glm::vec3(0.0, 0.0, 10), 1.f, color);
	//objects.PushTriangleBack(object1);

	//object1 = std::make_unique<triangle>();
	//object1->create(glm::vec3(0.0, -0.5, 0.0), 0.5f, color);
	//objects.PushTriangleBack(object1);

	std::srand(time(NULL));
	std::unique_ptr<triangle> object1;
	int minus[2] = { 1, 1 };
	for (int i = 0; i < 10000000; i++)
	{
		if (i % 100000 == 0) {
			for (int& a : minus)
			{
				switch (rand() % 2+1)
				{
					case 1:
					{
						a = 1;
						break;
					}
					case 2:
					{
						a = -1;
						break;
					}
				}
			}
			object1 = std::make_unique<triangle>();
			std::cout << "Object no: " << objects.GetTriangleSize()+1 << std::endl;
			color[0] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) };
			color[1] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
			color[2] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };

			object1->create(glm::vec3(((float)rand() / (RAND_MAX))*(rand()%2+1)*minus[0], ((float)rand() / (RAND_MAX)) * (rand() % 2 + 1) * minus[1], 0.0), 0.5f, color);
			objects.PushTriangleBack(object1);

		}
	}




	 std::vector<Vertex> vertices = {
{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
	};



	//initVertices->createVertexBuffer(vertices);

	 std::vector<uint16_t> indices = {
0, 1, 2, 2, 3, 0
	};
	//initIndices->createIndexBuffer(indices);

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
		initCommandPool->drawFrame(objects);

		if (initInput->keyState[KEY_ESCAPE].press == true) {
			break;
		}
		
		initInput->ClearKeyState();
	}

	initVulkan->WaitIdle();
}

void TestPlatform::CleanUp() {

	objects.destroy();

	initVulkan->cleanupSwapChain();

	initUniform->cleanup();

	//initIndices->cleanup();

	//initVertices->cleanup();

	initCommandPool->cleanup();

	initVulkan->cleanup();

	window->destroyWindow();
}