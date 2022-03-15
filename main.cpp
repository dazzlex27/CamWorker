#include <thread>
#include <atomic>
#include <functional>

#include "FrameQueue.h"
#include "FrameDispatcher.h"
#include "FrameProcessors/WorkerOneFrameProcessor.h"
#include "FrameProcessors/WorkerTwoFrameProcessor.h"

std::vector<std::shared_ptr<cw::FrameProcessor>> CreateWorkers()
{
	std::vector<std::shared_ptr<cw::FrameProcessor>> workers;
	workers.reserve(2);
	workers.emplace_back(std::make_shared<cw::WorkerOneFrameProcessor>());
	workers.emplace_back(std::make_shared<cw::WorkerTwoFrameProcessor>());

	return workers;
}

// since all highgui stuff only works properly when called from the main thread,
// all of those calls will be kept here for clarity
int main()
{
	std::cout << "starting frame processing..." << std::endl;

	cv::VideoCapture cap(0); // can only be created from the main (UI) thread
	cw::FrameDispatcher dispatcher(cap);
	
	auto workers = CreateWorkers();
	for (const auto& worker : workers)
	{
		dispatcher.AddSubscriber(worker);
		cv::namedWindow(worker->GetName(), cv::WINDOW_AUTOSIZE);
	}

	bool runLoop = true;
	dispatcher.Start();

	// poll workers for processed frames and display those frames as soon as they are ready
	while (dispatcher.IsActive() && runLoop)
	{
		if (workers.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100)); // avoid high CPU usage
			continue;
		}

		for (auto& worker : workers)
		{
			bool success = false;
			const auto nextFrame = worker->TryGetNextFrame(success);
			if (success)
				cv::imshow(worker->GetName(), nextFrame.GetImageData());
		}

		const auto key = cv::waitKey(1); // only works when invoked from the main (UI) thread
		if (key != -1) // press any key to exit
			runLoop = false;
	}

	cv::destroyAllWindows(); // clean up

	std::cout << "all done!" << std::endl;

	return 0;
}