#include "TestPlatform.h"

void TestPlatform::run() {
	Init();
	//window->main();
	mainLoop();
	CleanUp();
}

void TestPlatform::Init() {
	window = new windowClass;
	window->initWindow("TestPlatform");
}

void TestPlatform::mainLoop() {
	while (!window->isWindowClosed()) {
		std::cout << window->m_Data.Height << " " << window->m_Data.Width << std::endl;
		window->PoolEvents();
	}

}

void TestPlatform::CleanUp() {
	window->destroyWindow();
}