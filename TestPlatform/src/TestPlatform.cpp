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

	objects = new Objects(MAX_FRAMES_IN_FLIGHT);

	initVulkan = new VulkanClass(window->m_window, objects->GetUniformBuffer());
	initVulkan->init();

	objects->Init();

	initCommandPool = new CommandPool(initVulkan, window, MAX_FRAMES_IN_FLIGHT);
	initCommandPool->createCommandPool();

	glm::vec3 color2[4] = { {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f, 0.0f} ,{0.0f, 0.0f, 1.0f}, {0,0,0} };
	glm::vec3 color3[4] = { {1.0f, 1.0f, 1.0f} , {1.0f, 1.0f, 1.0f} ,{1.0f, 1.0f, 1.0f}, {1,1,1} };

	//objects->PushBack(Entity::rectangle::create(glm::vec3(0, 0, 0), 1, color2));
	//objects->PushBack(Entity::rectangle::create(glm::vec3(0, 0, -1.5f), 1, color2));
	//objects->PushBack(Entity::rectangle::create(glm::vec3(0.5, 0, 0.f), 1, color3));

	TestPlatform::AddEtities();

	initCommandPool->createCommandBuffers();
	initCommandPool->createSyncObjects();
}

void TestPlatform::AddEtities() {
	glm::vec3 color[3] = { {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f, 0.0f} ,{0.0f, 0.0f, 1.0f} };
	glm::vec3 color2[4] = { {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f, 0.0f} ,{0.0f, 0.0f, 1.0f}, {0,0,0} };
	glm::vec3 testVertex[4] = { {-0.5f, -0.5f, 0.0f} , {0.5f, -0.5, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.f} };


	std::srand(time(NULL));

	int minus[2] = { 1, 1 };
	bool tak = true;
	for (int i = 0; i < 5000000; i++)
	{
		if (i % 100000 == 0) {
			for (int& a : minus)
			{
				switch (rand() % 2 + 1)
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
				std::cout << "Object no: " << objects->GetEntitiesCount() + 1 << std::endl;
				color[0] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) };
				color[1] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
				color[2] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };

				objects->PushBack(Entity::triangle::create(glm::vec3(((float)rand() / (RAND_MAX)) * minus[0], ((float)rand() / (RAND_MAX)) * minus[1], ((float)rand() / (RAND_MAX)) * minus[1]), 0.5f, color));

				tak = false;
			}
			else {
				std::cout << "Object no: " << objects->GetEntitiesCount() + 1 << std::endl;
				color2[0] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) };
				color2[1] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
				color2[2] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
				color2[3] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };

				objects->PushBack(Entity::rectangle::create(glm::vec3(((float)rand() / (RAND_MAX)) * minus[0], ((float)rand() / (RAND_MAX)) * minus[1], ((float)rand() / (RAND_MAX)) * minus[1]),
					(float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), color2));
				tak = true;
			}
		}

	}
}

void TestPlatform::OnUpdate() {

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)initVulkan->GetSwapChainExtent().width / (float)initVulkan->GetSwapChainExtent().height, 0.1f, 10.0f);
	proj[1][1] *= -1;

	objects->UpdateView(view);
	objects->UpdateProjection(proj);

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	for (int i = 0; i < objects->GetEntitiesCount(); i++) {
		objects->rotate(i, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void TestPlatform::mainLoop() {
	while (!window->isWindowClosed()) {
		//std::cout << window->m_Data.Height << " " << window->m_Data.Width << std::endl;
		window->PoolEvents();
		window->setWindowFPS();

		TestPlatform::OnUpdate();

		initCommandPool->drawFrame(*objects);

		if (initInput->keyState[KEY_ESCAPE].press == true) {
			break;
		}
		
		initInput->ClearKeyState();
	}

	initVulkan->WaitIdle();
}

void TestPlatform::CleanUp() {

	objects->destroy();

	initVulkan->cleanupSwapChain();


	initCommandPool->cleanup();

	initVulkan->cleanup();

	window->destroyWindow();
}