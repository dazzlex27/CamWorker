#include "FrameDispatcher.h"
#include <thread>

using namespace cw;

FrameDispatcher::FrameDispatcher(cv::VideoCapture& capture, const uint queueCapacity)
	: _capture(capture), _queue(queueCapacity)
{
}

FrameDispatcher::~FrameDispatcher()
{
	Stop();
	RemoveAllSubscribers();
}

void FrameDispatcher::Start()
{
	if (_isFeedActive)
		return;

	_isFeedActive = true;
	_thRead = std::thread(&FrameDispatcher::RunFeedReading, this);
	_thNotify = std::thread(&FrameDispatcher::RunSubscriberNotification, this);
}

void FrameDispatcher::Stop()
{
	if (!_isFeedActive)
		return;

	_isFeedActive = false;
	_thRead.join();
	_thNotify.join();
}

void FrameDispatcher::AddSubscriber(const std::shared_ptr<FrameProcessor>& processor)
{
	_subscribers.emplace_back(processor);
}

void FrameDispatcher::RemoveAllSubscribers()
{
	_subscribers.clear();
}

void FrameDispatcher::RunFeedReading()
{
	std::cout << "starting feed reading thread..." << std::endl;

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

	std::cout << "feed reading thread finished" << std::endl;
}

void FrameDispatcher::RunSubscriberNotification()
{
	std::cout << "starting subscriber notification thread..." << std::endl;

	while (_isFeedActive)
	{
		if (_subscribers.empty())
		{
			// avoid high CPU usage when queue is empty
			std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
			continue;
		}

		bool success = false;
		Frame nextFrame = _queue.TryPop(success);
		if (!success)
			continue;

		for (auto& subscriber : _subscribers)
			subscriber->PushFrame(nextFrame);
	}

	std::cout << "subscriber notification thread finished" << std::endl;
}