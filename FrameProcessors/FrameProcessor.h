#pragma once

#include "../FrameQueue.h"

namespace cw
{
	class FrameProcessor
	{
	private:
		FrameQueue _incomingFrameQueue;
		FrameQueue _processedFrameQueue;
		std::thread _thProcessing;
		std::atomic<bool> _runFrameProcessing;

	public:
		FrameProcessor();
		FrameProcessor(const FrameProcessor& other);
		~FrameProcessor();

		virtual void PushFrame(const Frame& frame);
		Frame TryGetNextFrame(bool& success);
		virtual std::string GetName() const = 0;

	private:
		void RunQueueProcessing();
		virtual Frame ProcessNextFrame(const Frame& frame) = 0;
	};
}