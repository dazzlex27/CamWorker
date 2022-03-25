#pragma once

#include <list>
#include <thread>
#include <functional>
#include "CircularQueue.h"
#include "FrameProcessors/FrameProcessor.h"
#include "Frame.h"
#include <opencv2/videoio.hpp>

namespace cw
{
	class FrameDispatcher
	{
	private:
		cv::VideoCapture& _capture;
		CircularQueue<Frame>& _queue;
		std::atomic<bool> _isFeedActive;
		std::thread _thRead;

	public:
		FrameDispatcher(cv::VideoCapture& capture, CircularQueue<Frame>& frameQueue);
		~FrameDispatcher();

		bool IsActive() const {	return _isFeedActive; }

		void Start();
		void Stop();

	private:
		void RunFrameDispatching();
	};
}