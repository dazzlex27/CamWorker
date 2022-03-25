#include "FrameDispatcher.h"
#include <thread>

using namespace cw;

FrameDispatcher::FrameDispatcher(cv::VideoCapture& capture, CircularQueue<Frame>& frameQueue)
	: _capture(capture), _queue(frameQueue)
{
}

FrameDispatcher::~FrameDispatcher()
{
	Stop();
}

void FrameDispatcher::Start()
{
	if (_isFeedActive)
		return;

	_isFeedActive = true;
	_thRead = std::thread(&FrameDispatcher::RunFrameDispatching, this);
}

void FrameDispatcher::Stop()
{
	if (!_isFeedActive)
		return;

	_isFeedActive = false;
	_thRead.join();
}

void FrameDispatcher::RunFrameDispatching()
{
	std::cout << "starting frame dispatching..." << std::endl;

	// set up specific video config if necessary
	//_capture.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
	//_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
	//_capture.set(cv::CAP_PROP_FPS, 25.0);

	// poll actual video params
	const double actualWidth = _capture.get(cv::CAP_PROP_FRAME_WIDTH);
	const double actualHeight = _capture.get(cv::CAP_PROP_FRAME_HEIGHT);
	const double actualFps = _capture.get(cv::CAP_PROP_FPS);
	std::cout << "Feed params: w=" << actualWidth << " h=" << actualHeight << " fps=" << actualFps << std::endl;

	while (_isFeedActive)
	{
		if (!_capture.isOpened())
		{
			std::cout << "video feed is lost!" << std::endl;
			break;
		}

		cv::Mat imageData;
		_capture.read(imageData);

		if (imageData.empty())
			continue;

		Frame frame(imageData, hrc::now());
		_queue.Push(frame);
	}

	std::cout << "frame dispatching finished" << std::endl;
}