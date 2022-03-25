#include "FrameProcessor.h"

using namespace cw;

FrameProcessor::FrameProcessor(CircularQueue<std::pair<std::string, Frame>>& processedFrameQueue)
: _incomingFrameQueue(1), _processedFrameQueue(processedFrameQueue), _runFrameProcessing(true)
{
	_thProcessing = std::thread(&FrameProcessor::RunQueueProcessing, this);
}

FrameProcessor::~FrameProcessor()
{
	_runFrameProcessing = false;
	_incomingFrameQueue.StopQueue();
	_thProcessing.join();
}

void FrameProcessor::PushFrame(const Frame& frame)
{
	_incomingFrameQueue.Push(frame);
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
		{
			std::pair<std::string, Frame> data(GetName(), processedFrame);
			_processedFrameQueue.Push(data);
		}
	}
}