#include "CamWorker.h"

int main()
{
	std::cout << "starting frame processing..." << std::endl;

	cw::CamWorker camWorker; // contains highgui code, can only be run from the main thread
	camWorker.Run(); // will run continiously until any key is pressed within a highgui window

	std::cout << "all done!" << std::endl;

	return 0;
}