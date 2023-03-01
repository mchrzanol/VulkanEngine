#pragma once

#include <iostream>
#include "Engine.h"

class TestPlatform{
public:
	void run();

private:
	windowClass* window;

	void Init();

	void OnUpdate();

	void OnRender();

	void mainLoop();

	void CleanUp();

	void Print();//Test
};
