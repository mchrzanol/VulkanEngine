#include "TestPlatform.h"

int main()
{
	TestPlatform* a = new TestPlatform;

	try
	{
		a->run();
	}
	//JD
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}