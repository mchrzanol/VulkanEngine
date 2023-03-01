#include "TestPlatform.h"

void TestPlatform::run() {
	Init();
	//window->main();
	mainLoop();
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
	window->destroyWindow();
}
