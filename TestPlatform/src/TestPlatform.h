#pragma once

#include <iostream>
#include "Engine.h"
#include <cstdlib>
#include <ctime>

const int MAX_FRAMES_IN_FLIGHT = 2;

class TestPlatform{
public:
	void run();

private:
	windowClass* window;
	VulkanClass* initVulkan;
	CommandPool* initCommandPool;
	Input* initInput;

	Objects* objects;


	void Init();

	void OnUpdate();

	void OnRender();

	void mainLoop();

	void CleanUp();
};
