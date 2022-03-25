#pragma once

#include <atomic>
#include <thread>
#include "../Frame.h"
#include "../CircularQueue.h"

namespace cw
{
	class FrameProcessor
	{
	private:
		CircularQueue<Frame> _incomingFrameQueue;
		CircularQueue<std::pair<std::string, Frame>>& _processedFrameQueue;
		std::thread _thProcessing;
		std::atomic<bool> _runFrameProcessing;

	public:
		FrameProcessor(CircularQueue<std::pair<std::string, Frame>>& processedFrameQueue);
		virtual ~FrameProcessor();

		virtual void PushFrame(const Frame& frame);
		virtual std::string GetName() const = 0;

	private:
		void RunQueueProcessing();
		virtual Frame ProcessNextFrame(const Frame& frame) = 0;
	};
}