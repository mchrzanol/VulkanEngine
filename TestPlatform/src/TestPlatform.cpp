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
	//initUniform->createUniform(0, sizeof(glm::mat4), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, TypeOfUniform::LocalUniform);

	initVulkan = new VulkanClass(window->m_window, initUniform);
	initVulkan->init();

	initCommandPool = new CommandPool(initVulkan, initUniform, window, MAX_FRAMES_IN_FLIGHT);
	initCommandPool->createCommandPool();


	glm::vec3 color[3] = { {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f, 0.0f} ,{0.0f, 0.0f, 1.0f} };
	glm::vec3 color2[4] = { {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f, 0.0f} ,{0.0f, 0.0f, 1.0f}, {0,0,0} };
	glm::vec3 testVertex[4] = { {-0.5f, -0.5f, 0.0f} , {0.5f, -0.5, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.f} };


	std::srand(time(NULL));
	triangle * tri;
	rectangle* rect;
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
				tri = new triangle();
				std::cout << "Object no: " << objects.GetTrianglesSize() + 1 << std::endl;
				color[0] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) };
				color[1] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
				color[2] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };

				tri->create(glm::vec3(((float)rand() / (RAND_MAX)) * minus[0], ((float)rand() / (RAND_MAX)) * minus[1], 0.0), 0.5f, color);
				objects.PushBack(tri);

				tak = false;

				delete tri;
				tri = nullptr;
			}
			else {
				rect = new rectangle();
				std::cout << "Object no: " << objects.GetRectanglesSize() + 1 << std::endl;
				color2[0] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) };
				color2[1] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
				color2[2] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
				color2[3] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };

				rect->create(glm::vec3(((float)rand() / (RAND_MAX)) * minus[0], ((float)rand() / (RAND_MAX)) * minus[1], 0.0), 
					(float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), color2);
				objects.PushBack(rect);

				tak = true;

				delete rect;
				rect = nullptr;
			}
		}

	}
	
	initUniform->createUniformBuffers();
	initUniform->createDescriptorPool();
	initUniform->createDescriptorSets();

	initCommandPool->createCommandBuffers();
	initCommandPool->createSyncObjects();
}

void TestPlatform::OnUpdate() {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	//ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.model = glm::mat4(1.f);

	ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//ubo.view = glm::mat4(1.f);

	ubo.proj = glm::perspective(glm::radians(90.0f), (float)initVulkan->GetSwapChainExtent().width / (float)initVulkan->GetSwapChainExtent().height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	initUniform->updateUniformBuffer(0, TypeOfUniform::GlobalUniform, ubo, initCommandPool->GetCurrentFrame());//maybe it doesn't work for arrays(ckeck it) read for dynamicUniforms
}

void TestPlatform::mainLoop() {
	while (!window->isWindowClosed()) {
		//std::cout << window->m_Data.Height << " " << window->m_Data.Width << std::endl;
		window->PoolEvents();
		window->setWindowFPS();

		TestPlatform::OnUpdate();

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

	initCommandPool->cleanup();

	initVulkan->cleanup();

	window->destroyWindow();
}