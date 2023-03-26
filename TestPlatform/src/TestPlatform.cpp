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

	initUniform->createUniform(0, sizeof(UniformBufferObject), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, TypeOfUniform::GlobalUniform);

	initVulkan = new VulkanClass(window->m_window, initUniform);
	initVulkan->init();

	initCommandPool = new CommandPool(initVulkan, initUniform, window, MAX_FRAMES_IN_FLIGHT);
	initCommandPool->createCommandPool();

	//std::unique_ptr<triangle> object1 = std::make_unique<triangle>();
	glm::vec3 color[3] = { {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f, 0.0f} ,{0.0f, 0.0f, 1.0f} };
	glm::vec3 color2[4] = { {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f, 0.0f} ,{0.0f, 0.0f, 1.0f}, {0,0,0} };
	glm::vec3 testVertex[4] = { {-0.5f, -0.5f, 0.0f} , {0.5f, -0.5, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.f} };

	//std::unique_ptr<rectangle> rect = std::make_unique<rectangle>();

	//rect->create(testVertex, color2);
	//objects.PushBack(rect);
	//rect = std::make_unique<rectangle>();
	//rect->create(glm::vec3(0.75, 0, 0), 1.f, 0.5f, color2);
	//objects.PushBack(rect);


	//object1->create(testVertex, color);
	//object1->create(glm::vec3(0.0, 0.0, 10), 1.f, color);
	//objects.PushTriangleBack(object1);

	//object1 = std::make_unique<triangle>();
	//object1->create(glm::vec3(0.0, -0.5, 0.0), 0.5f, color);
	//objects.PushTriangleBack(object1);

	//objects.PushBack<VertexBuffer>();

	std::srand(time(NULL));
	std::unique_ptr<triangle> tri;
	std::unique_ptr<rectangle> rect;
	int minus[2] = { 1, 1 };
	bool tak = true;
	for (int i = 0; i < 5000000; i++)
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
			if (tak == true) {
				tri = std::make_unique<triangle>();
				std::cout << "Object no: " << objects.GetTrianglesSize() + 1 << std::endl;
				color[0] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) };
				color[1] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
				color[2] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };

				tri->create(glm::vec3(((float)rand() / (RAND_MAX)) * minus[0], ((float)rand() / (RAND_MAX)) * minus[1], 0.0), 0.5f, color);
				objects.PushBack(tri);

				tak = false;
			}
			else {
				rect = std::make_unique<rectangle>();
				std::cout << "Object no: " << objects.GetRectanglesSize() + 1 << std::endl;
				color2[0] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) };
				color2[1] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
				color2[2] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
				color2[3] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };

				rect->create(glm::vec3(((float)rand() / (RAND_MAX)) * minus[0], ((float)rand() / (RAND_MAX)) * minus[1], 0.0), 
					(float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), color2);
				objects.PushBack(rect);

				tak = true;
			}
		}

	}
	
	initUniform->createUniformBuffers();
	initUniform->createDescriptorPool();
	initUniform->createDescriptorSets();

	initCommandPool->createCommandBuffers();
	initCommandPool->createSyncObjects();
}

void TestPlatform::mainLoop() {
	while (!window->isWindowClosed()) {
		//std::cout << window->m_Data.Height << " " << window->m_Data.Width << std::endl;
		window->PoolEvents();
		window->setWindowFPS();
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