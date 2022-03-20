#pragma once

#include "../Frame.h"
#include "../CircularQueue.h"

namespace cw
{
	class FrameProcessor
	{
	private:
		CircularQueue<Frame> _incomingFrameQueue;
		CircularQueue<Frame> _processedFrameQueue;
		std::thread _thProcessing;
		std::atomic<bool> _runFrameProcessing;

	public:
		FrameProcessor();
		FrameProcessor(const FrameProcessor& other);
		virtual ~FrameProcessor();

		virtual void PushFrame(const Frame& frame);
		Frame TryGetNextFrame(bool& success);
		virtual std::string GetName() const = 0;

	private:
		void RunQueueProcessing();
		virtual Frame ProcessNextFrame(const Frame& frame) = 0;
	};
}