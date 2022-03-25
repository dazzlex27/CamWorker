#include "CamWorker.h"
#include "FrameDispatcher.h"
#include "FrameProcessors/WorkerOneFrameProcessor.h"
#include "FrameProcessors/WorkerTwoFrameProcessor.h"
#include <opencv2/highgui.hpp>

using namespace cw;

CamWorker::CamWorker()
	: _runLoop(false), _rawFrameQueue(10), _procesedFrameQueue(30)
{
	_workers = CreateWorkers();
}

void CamWorker::Run()
{
	if (_workers.empty())
	{
		std::cout << "no workers were created, exiting..." << std::endl;
		return;
	}

	cv::VideoCapture cap(0); // can only be created from the main (UI) thread
	FrameDispatcher dispatcher(cap, _rawFrameQueue);

	for (const auto& worker : _workers)
		cv::namedWindow(worker->GetName(), cv::WINDOW_AUTOSIZE);

	_runLoop = true;
	dispatcher.Start();
	_thNofify = std::thread(&CamWorker::RunWorkerNotification, this);

	// poll workers for processed frames and display those frames as soon as they are ready
	while (dispatcher.IsActive() && _runLoop)
	{
		std::pair<std::string, Frame> frameData = _procesedFrameQueue.Pop();
		cv::imshow(frameData.first, frameData.second.GetImageData());

		const auto key = cv::waitKey(1); // only works when invoked from the main (UI) thread
		if (key != -1) // press any key to exit
			_runLoop = false;
	}

	Stop();
}

// create and add custom workers here
std::vector<std::shared_ptr<FrameProcessor>> CamWorker::CreateWorkers()
{
	std::vector<std::shared_ptr<cw::FrameProcessor>> workers;
	workers.reserve(2);
	workers.emplace_back(std::make_shared<cw::WorkerOneFrameProcessor>(_procesedFrameQueue));
	workers.emplace_back(std::make_shared<cw::WorkerTwoFrameProcessor>(_procesedFrameQueue));

	return workers;
}

void CamWorker::RunWorkerNotification()
{
	std::cout << "starting worker notification..." << std::endl;

	while (_runLoop)
	{
		Frame nextFrame = _rawFrameQueue.Pop();

		for (auto& worker : _workers)
			worker->PushFrame(nextFrame);
	}

	std::cout << "worker notification finished" << std::endl;
}

void CamWorker::Stop()
{
	_runLoop = false;
	_rawFrameQueue.StopQueue();
	_procesedFrameQueue.StopQueue();
	_thNofify.join();
	cv::destroyAllWindows();
}