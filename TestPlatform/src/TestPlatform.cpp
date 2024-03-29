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
	initInput = new Input(window->m_window, &window->m_Data.Width, &window->m_Data.Height);
	window->addUserPointer(initInput);

	objects = new Objects(MAX_FRAMES_IN_FLIGHT);

	VulkanCore = new VulkanStruct(window);
	VulkanCore->Init(objects->GetUniformBuffer());

	initCommandPool = new CommandPool(VulkanCore, window, MAX_FRAMES_IN_FLIGHT);
	initCommandPool->createCommandPool();

	objects->Init(VulkanCore, initCommandPool->GetCommandPool());


	//objects->PushBack(Entity::rectangle::create(glm::vec3(0, 0, -0.5), 2, Orientation::Z, glm::vec3(1,1,1)));

	objects->PushBack(Entity::rectangle::create(glm::vec3(0.5, 0, -0.5), 1, Orientation::Z, "dupa"));
	//objects->PushBack(Entity::rectangle::create(glm::vec3(0, 0, 4), 10, Orientation::Z, "invalid"));
	//objects->PushBack(Entity::rectangle::create(glm::vec3(0, 1, -0.5), 1, 1.5, Orientation::Z));
	//objects->PushBack(Entity::rectangle::create(glm::vec3(0, 0.5,0), 2));
	//objects->PushBack(Entity::triangle::create(glm::vec3(0, -1, 0), 1, Orientation::X, glm::vec3(1, 0, 0)));
	//objects->PushBack(Entity::rectangle::create(glm::vec3(0, 0.5, 0), 2, Orientation::Y));


	//TestPlatform::AddEtities();

	initCommandPool->createCommandBuffers();
	initCommandPool->createSyncObjects();

	startTime = std::chrono::high_resolution_clock::now();
}

void TestPlatform::AddEtities() {
	glm::vec3 color[3] = { {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f, 0.0f} ,{0.0f, 0.0f, 1.0f} };
	glm::vec3 color2[4] = { {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f, 0.0f} ,{0.0f, 0.0f, 1.0f}, {0,0,0} };
	glm::vec3 testVertex[4] = { {-0.5f, -0.5f, 0.0f} , {0.5f, -0.5, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.f} };


	std::srand(time(NULL));

	int minus[2] = { 1, 1 };
	bool tak = true;
	for (int i = 0; i < 90000000; i++)
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

				//objects->PushBack(Entity::triangle::create(glm::vec3(((float)rand() / (RAND_MAX)) * minus[0], ((float)rand() / (RAND_MAX)) * minus[1], ((float)rand() / (RAND_MAX)) * minus[1]), 0.5f, Orientation::Z, color[0]));

				tak = false;
			}
			else {
				std::cout << "Object no: " << objects->GetEntitiesCount() + 1 << std::endl;
				color2[0] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) };
				color2[1] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
				color2[2] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };
				color2[3] = { (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX) , (float)rand() / (RAND_MAX) };

				objects->PushBack(Entity::rectangle::create(glm::vec3(((float)rand() / (RAND_MAX)) * minus[0], ((float)rand() / (RAND_MAX)) * minus[1], ((float)rand() / (RAND_MAX)) * minus[1]),(float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), Orientation::Z, "invalid"));
				tak = true;
			}
		}

	}
}

void TestPlatform::OnUpdate() {

	glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)VulkanCore->GetSwapChainExtent().width / (float)VulkanCore->GetSwapChainExtent().height, 0.1f, 100.0f);
	proj[1][1] *= -1;

	objects->UpdateView(view.GetView());
	objects->UpdateProjection(proj);

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	if (time >= 0.2) {
		startTime = currentTime;
	//	objects->rotate();
	}
}

void TestPlatform::mainLoop() {
	while (!window->isWindowClosed()) {
		//std::cout << window->m_Data.Height << " " << window->m_Data.Width << std::endl;
		window->PoolEvents();
		window->setWindowFPS();

		initInput->MouseEvents();


		TestPlatform::OnUpdate();

		if (initInput->keyState[KEY_ESCAPE].press == true) {
			break;
		}
		if (initInput->keyState[KEY_W].press == true) {
			view.ChangePosz(0.05f);
		}

		if (initInput->keyState[KEY_S].press == true) {
			view.ChangePosz(-0.05f);
		}

		if (initInput->keyState[KEY_A].press == true) {
			view.ChangePosx(-0.05f);
		}

		if (initInput->keyState[KEY_D].press == true) {
			view.ChangePosx(0.05f);
		}

		if (initInput->keyState[KEY_P].release == true) {
			initInput->CursorLock = !initInput->CursorLock;
		}
		if (initInput->keyState[KEY_E].release == true) {
			objects->addTexture("dupa", "../textures/test.jpg");
			//std::cout << "Object no: " << objects->GetEntitiesCount() + 1 << std::endl;
			//objects->PushBack(Entity::rectangle::create(glm::vec3(2, 0, -0.5), 1, Orientation::Z));
			//objects->PushBack(Entity::rectangle::create(glm::vec3(((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)) , ((float)rand() / (RAND_MAX))*3 ), (float)rand() / (RAND_MAX), (float)rand() / (RAND_MAX), Orientation::Z));
		}
		if (initInput->keyState[KEY_R].release == true) {
			objects->addTexture("invalid", "../textures/pudzian.jpg");
		}

		view.UpdateFront({ initInput->deltaCursorPos.x, initInput->deltaCursorPos.y });


		view.update();

		initCommandPool->drawFrame(*objects);
		initInput->ClearKeyState();
	}

	VulkanCore->WaitIdle();
}

void TestPlatform::CleanUp() {

	objects->destroy();

	initCommandPool->cleanup();

	VulkanCore->cleanup();

	window->destroyWindow();
}