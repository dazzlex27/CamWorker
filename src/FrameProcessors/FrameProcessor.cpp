#include "FrameProcessor.h"

using namespace cw;

FrameProcessor::FrameProcessor()
	: _incomingFrameQueue(10), _processedFrameQueue(30), _runFrameProcessing(true)
{
	_thProcessing = std::thread(&FrameProcessor::RunQueueProcessing, this);
}

FrameProcessor::FrameProcessor(const FrameProcessor& other)
	: _incomingFrameQueue(other._incomingFrameQueue.GetCapacity()),	_processedFrameQueue(30), _runFrameProcessing(true)
{
}

FrameProcessor::~FrameProcessor()
{
	_runFrameProcessing = false;
	_incomingFrameQueue.StopQueue();
	_processedFrameQueue.StopQueue();
	_thProcessing.join();
}

void FrameProcessor::PushFrame(const Frame& frame)
{
	_incomingFrameQueue.Push(frame);
}

Frame cw::FrameProcessor::TryGetNextFrame(bool& success)
{
	return _processedFrameQueue.TryPop(success);
}

void cw::FrameProcessor::RunQueueProcessing()
{
	while (_runFrameProcessing)
	{
		Frame nextFrame = _incomingFrameQueue.Pop();
		if (nextFrame.GetImageData().empty())
			continue;

		Frame processedFrame = ProcessNextFrame(nextFrame);
		if (!processedFrame.GetImageData().empty())
			_processedFrameQueue.Push(processedFrame);
	}
}